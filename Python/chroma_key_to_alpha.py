
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
단색 크로마키 배경을 투명 처리하는 스크립트.
- 입력: 파일 또는 폴더
- 크로마키 색상: #RRGGBB / green / blue / auto(모서리 샘플)
- 임계값(threshold)과 feather(부드럽게) 지원
- 간단한 de-spill(색 번짐 억제) 지원
필요 패키지: Pillow
    pip install pillow
"""

import argparse
from pathlib import Path
from typing import Tuple, Optional, List

from PIL import Image, ImageFilter
import numpy as np


def parse_color(s: str) -> Tuple[int, int, int]:
    s = s.strip().lower()
    if s in ("green", "g"):
        return (0, 255, 0)
    if s in ("blue", "b"):
        return (0, 0, 255)
    if s.startswith("#") and len(s) == 7:
        return tuple(int(s[i:i+2], 16) for i in (1, 3, 5))  # type: ignore
    # "r,g,b" 형태 허용
    if "," in s:
        parts = s.split(",")
        if len(parts) == 3:
            return tuple(int(p) for p in parts)  # type: ignore
    raise ValueError("색상은 #RRGGBB, 'green', 'blue', 또는 'R,G,B' 형식이어야 합니다.")


def auto_key_color(img: Image.Image, border: int = 10) -> Tuple[int, int, int]:
    """
    모서리/테두리 영역을 샘플링하여 가장 흔한 색(또는 중앙값)에 가깝게 추정.
    """
    rgba = img.convert("RGBA")
    arr = np.array(rgba)
    h, w, _ = arr.shape
    b = max(2, min(border, min(h, w)//4))

    # 위/아래/좌/우 테두리 샘플
    samples = np.concatenate([
        arr[0:b, :, :3].reshape(-1, 3),
        arr[h-b:h, :, :3].reshape(-1, 3),
        arr[:, 0:b, :3].reshape(-1, 3),
        arr[:, w-b:w, :3].reshape(-1, 3),
    ], axis=0)

    # 알파가 있는 경우 불투명 가까운 픽셀만 사용(배경이 완전 불투명일 때 더 정확)
    # 여기서는 단순히 RGB만 사용
    # 중앙값을 쓰면 조명 변화에 조금 더 견고
    med = np.median(samples, axis=0).astype(np.uint8)
    return (int(med[0]), int(med[1]), int(med[2]))


def smoothstep(edge0: float, edge1: float, x: np.ndarray) -> np.ndarray:
    # 0~1로 정규화 후 스무스 곡선 적용
    t = np.clip((x - edge0) / max(1e-8, (edge1 - edge0)), 0.0, 1.0)
    return t * t * (3.0 - 2.0 * t)


def chroma_to_alpha_core(
    img: Image.Image,
    key_rgb: Tuple[int, int, int],
    threshold: float,
    feather: float,
    despill: float,
) -> Image.Image:
    """
    RGB 거리 기반 키잉:
    - d = ||RGB - key|| (유클리드)
    - d <= (thr - feather): 완전 투명
    - d >= thr: 원본 알파 유지(완전 불투명)
    - 그 사이: 스무스하게 보간
    """
    rgba = img.convert("RGBA")
    arr = np.array(rgba).astype(np.float32)
    rgb = arr[:, :, :3]
    a = arr[:, :, 3:4] / 255.0

    key = np.array(key_rgb, dtype=np.float32).reshape(1, 1, 3)
    dist = np.linalg.norm(rgb - key, axis=2)  # 0~441.67(=sqrt(3*255^2))

    t0 = max(0.0, threshold - feather)
    t1 = threshold

    # alpha_factor: 0(완전 투명) ~ 1(원본 알파)
    alpha_factor = smoothstep(t0, t1, dist)[..., None]

    # 새 알파 = 원래 알파 * alpha_factor
    new_a = a * alpha_factor

    # 간단한 de-spill: 키 색상 성분을 (1 - alpha_factor)에 비례하여 눌러주기
    # 키 색상에서 가장 큰 채널을 찾아 그 채널을 살짝 줄임
    if despill > 0.0:
        key_dom = int(np.argmax(key.flatten()))
        # 스필량 = (1 - alpha_factor) * despill * (해당 채널이 주변보다 큰 정도)
        # 단순하게 해당 채널을 회색(다른 채널 평균) 쪽으로 이동
        other_idx = [0, 1, 2]
        other_idx.remove(key_dom)
        other_mean = np.mean(rgb[:, :, other_idx], axis=2, keepdims=True)
        spill_strength = (1.0 - alpha_factor) * despill
        # 키 채널만 보정
        rgb_dom = rgb[:, :, key_dom:key_dom+1]
        rgb[:, :, key_dom:key_dom+1] = rgb_dom * (1.0 - spill_strength) + other_mean * spill_strength

    # 합치기
    out = np.concatenate([np.clip(rgb, 0, 255), np.clip(new_a * 255.0, 0, 255)], axis=2).astype(np.uint8)
    out_img = Image.fromarray(out, mode="RGBA")

    # 가장자리 부드러움 추가(선택): feather가 있으면 알파만 살짝 가우시안
    if feather > 0:
        # α 채널만 블러 → 가장자리 살짝 부드럽게
        a_img = out_img.split()[3].filter(ImageFilter.GaussianBlur(radius=max(0.0, feather * 0.2)))
        out_img.putalpha(a_img)

    return out_img


def process_one(
    in_path: Path,
    out_path: Path,
    key: Optional[Tuple[int, int, int]],
    threshold: float,
    feather: float,
    despill: float,
    auto_border: int,
):
    img = Image.open(in_path)
    key_rgb = auto_key_color(img, border=auto_border) if key is None else key

    result = chroma_to_alpha_core(
        img=img,
        key_rgb=key_rgb,
        threshold=threshold,
        feather=feather,
        despill=despill,
    )
    out_path.parent.mkdir(parents=True, exist_ok=True)

    # 지원되는 출력 확장자
    valid_exts = {".png", ".bmp"}
    ext = out_path.suffix.lower()
    if ext not in valid_exts:
        out_path = out_path.with_suffix(".png")

    # BMP의 경우: 알파 채널을 흰색 배경으로 합성
    if out_path.suffix.lower() == ".bmp":
        background_color = (255, 255, 255)  # 흰색 배경
        bg = Image.new("RGB", result.size, background_color)
        bg.paste(result, mask=result.split()[3])  # 알파 채널을 마스크로 사용
        bg.save(out_path, format="BMP")
    else:
        # PNG는 알파 그대로 저장
        result.save(out_path, format="PNG")

    print(f"[OK] {in_path.name} -> {out_path.name}  key={key_rgb}")


def iter_images(dir_path: Path) -> List[Path]:
    exts = {".png", ".jpg", ".jpeg", ".bmp", ".tga", ".webp"}
    return [p for p in dir_path.rglob("*") if p.suffix.lower() in exts]


def main():
    ap = argparse.ArgumentParser(description="단색 크로마키 배경 투명 처리")
    ap.add_argument("input", help="입력 파일 또는 폴더 경로")
    ap.add_argument("output", help="출력 파일 또는 폴더 경로")
    ap.add_argument(
        "--key",
        default="auto",
        help="크로마키 색상: #RRGGBB / 'green' / 'blue' / 'R,G,B' / 'auto'(기본)",
    )
    ap.add_argument(
        "--threshold",
        type=float,
        default=90.0,
        help="색 거리 임계값(0~441). 값이 클수록 더 많은 픽셀이 남음(기본 90)",
    )
    ap.add_argument(
        "--feather",
        type=float,
        default=12.0,
        help="가장자리 부드럽게 정도(픽셀 단위, 기본 12). 0이면 날카롭게.",
    )
    ap.add_argument(
        "--despill",
        type=float,
        default=0.25,
        help="스필 억제 강도(0~1, 기본 0.25). 0이면 비활성.",
    )
    ap.add_argument(
        "--auto-border",
        type=int,
        default=10,
        help="auto 키색 추정 시 테두리 샘플 두께(픽셀, 기본 10)",
    )

    args = ap.parse_args()
    in_path = Path(args.input)
    out_path = Path(args.output)

    key = None if args.key.lower() == "auto" else parse_color(args.key)

    if in_path.is_dir():
        imgs = iter_images(in_path)
        if not imgs:
            print("이미지 파일이 없습니다.")
            return
        # 출력이 폴더가 아니면 폴더로 사용
        out_dir = out_path if out_path.suffix == "" else out_path.parent
        for p in imgs:
            rel = p.relative_to(in_path)
            process_one(
                p,
                out_dir / rel.with_suffix(".png"),
                key,
                args.threshold,
                args.feather,
                args.despill,
                args.auto_border,
            )
    else:
        # 단일 파일
        if out_path.is_dir():
            out_file = out_path / (in_path.stem + ".png")
        else:
            out_file = out_path
        process_one(
            in_path,
            out_file,
            key,
            args.threshold,
            args.feather,
            args.despill,
            args.auto_border,
        )


if __name__ == "__main__":
    main()
