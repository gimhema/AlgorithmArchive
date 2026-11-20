# Rust Tutorial for C++ Developers

C++ 개발자가 "C++에서는 이렇게 했는데, Rust에서는 어떻게 하지?"를 빠르게 해결할 수 있도록 구성한 튜토리얼입니다.
문법의 세세한 부분보다는 **실무에서 자주 쓰는 패턴** 위주로 정리했습니다.

## 구성

| 단계 | 문서 | 주요 내용 |
|------|------|-----------|
| 초급 | [01_Beginner.md](01_Beginner.md) | Cargo, 변수/타입, 소유권·빌림, String/Vec, struct/enum/match, Option/Result, 모듈 |
| 중급 | [02_Intermediate.md](02_Intermediate.md) | Trait, Generic, 동적 디스패치, 스마트 포인터, 반복자·클로저, 에러 처리, 라이프타임 기초, 테스트 |
| 고급 | [03_Advanced.md](03_Advanced.md) | 라이프타임 심화, 동시성(Send/Sync), async, 매크로, unsafe/FFI, 설계 패턴, Cargo 실무 |

## 한눈에 보는 C++ → Rust 대응표

| C++ | Rust | 비고 |
|-----|------|------|
| `const int x = 1;` / `int x = 1;` | `let x = 1;` / `let mut x = 1;` | Rust는 **기본이 불변** |
| `constexpr` | `const`, `const fn` | |
| `T&` / `const T&` | `&mut T` / `&T` | 컴파일러가 규칙을 강제 |
| `std::move(x)` | 그냥 `x` (기본이 move) | move 후 원본 사용 시 컴파일 에러 |
| 복사 생성자 | `.clone()` (명시적) / `Copy` trait | 암묵적 깊은 복사 없음 |
| 소멸자 `~T()` | `impl Drop for T` | RAII 동일 |
| `std::unique_ptr<T>` | `Box<T>` | |
| `std::shared_ptr<T>` | `Rc<T>` (단일 스레드) / `Arc<T>` (멀티 스레드) | |
| `std::weak_ptr<T>` | `Weak<T>` | |
| `mutable` 멤버 | `Cell<T>` / `RefCell<T>` | 내부 가변성 |
| `std::optional<T>` / `nullptr` | `Option<T>` | null 없음 |
| 예외 / `std::expected<T,E>` | `Result<T, E>` + `?` | 예외 없음 (panic은 복구 불가 오류용) |
| `enum class` + `std::variant` | `enum` (데이터를 담을 수 있음) + `match` | |
| `class` + 멤버 함수 | `struct` + `impl` | 상속 없음 |
| 추상 클래스 / `virtual` | `trait` + `dyn Trait` | |
| 템플릿 / C++20 concepts | 제네릭 + trait bound | |
| `std::vector<T>` | `Vec<T>` | |
| `std::string` / `std::string_view` | `String` / `&str` | UTF-8 보장 |
| `std::span<T>` | `&[T]` / `&mut [T]` | |
| `std::unordered_map` / `std::map` | `HashMap` / `BTreeMap` | |
| 람다 `[&](int x){...}` | 클로저 `\|x\| ...` | |
| `<algorithm>` / ranges | Iterator 어댑터 (`map`, `filter`, ...) | |
| `std::thread`, `std::mutex` | `std::thread`, `Mutex<T>` | Mutex가 데이터를 **소유** |
| `#include` / namespace | `mod` / `use` | |
| `#ifdef FEATURE` | `#[cfg(feature = "...")]` | |
| CMake + vcpkg/conan | Cargo | 빌드/의존성/테스트/문서 일체형 |
| 매크로 `#define` | `macro_rules!` / proc-macro | 위생적(hygienic) 매크로 |
| `operator+` | `impl Add for T` | |
| `operator<<(ostream&)` | `impl Display for T` | |
| `extern "C"` | `extern "C"` + `#[no_mangle]` | FFI |

## 추천 학습 순서

1. 초급 문서를 읽으며 `cargo new playground`로 예제를 직접 돌려보기
2. 컴파일 에러 메시지를 **꼼꼼히 읽기** — Rust 컴파일러 메시지는 대부분 해결책까지 알려줍니다
3. 중급에서 trait / 스마트 포인터 / 에러 처리를 익히면 실무 코드 대부분을 읽을 수 있음
4. 고급은 필요할 때 레퍼런스처럼 참고

## 참고 자료

- The Rust Programming Language (The Book): https://doc.rust-lang.org/book/
- Rust by Example: https://doc.rust-lang.org/rust-by-example/
- Rust API Guidelines: https://rust-lang.github.io/api-guidelines/
- The Rustonomicon (unsafe): https://doc.rust-lang.org/nomicon/
