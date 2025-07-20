{-# LANGUAGE TemplateHaskell #-}

import Graphics.Rendering.Chart.Easy
import Graphics.Rendering.Chart.Backend.Diagrams
import Data.Default.Class (def)

-- 타입 정의 (라이브러리와의 이름 충돌을 피하기 위해 MyVector/MyMatrix로 변경)
type MyVector = [Double]
type MyMatrix = [[Double]]

-- 벡터 연산
-- 벡터 덧셈
vAdd :: MyVector -> MyVector -> MyVector
vAdd [x1, y1] [x2, y2] = [x1 + x2, y1 + y2]
vAdd _ _ = error "2D 벡터만 지원합니다."

-- 스칼라 곱셈
vScale :: Double -> MyVector -> MyVector
vScale s [x, y] = [s * x, s * y]
vScale _ _ = error "2D 벡터만 지원합니다."

-- 벡터 크기
vLength :: MyVector -> Double
vLength [x, y] = sqrt (x^2 + y^2)

-- 단위 벡터 (정규화)
normalize :: MyVector -> MyVector
normalize v@[x, y] = let len = vLength v in if len == 0 then v else [x / len, y / len]

-- 행렬-벡터 곱셈
matVecMul :: MyMatrix -> MyVector -> MyVector
matVecMul [[a, b], [c, d]] [x, y] = [a*x + b*y, c*x + d*y]
matVecMul _ _ = error "2x2 행렬과 2D 벡터만 지원합니다."

-- 리스트를 튜플로 변환 (차트용)
toTuple :: MyVector -> (Double, Double)
toTuple [x, y] = (x, y)
toTuple _ = error "2D 벡터만 지원합니다."

-- 2x2 행렬의 고윳값 찾기
-- 특성방정식: λ^2 - tr(A)λ + det(A) = 0
findEigenvalues2x2 :: MyMatrix -> [Double]
findEigenvalues2x2 m@[[a, b], [c, d]] =
  let trace = a + d
      det = a*d - b*c
      discriminant = trace^2 - 4*det
  in if discriminant < 0
     then [] -- 실수 고윳값이 없는 경우 (복소수)
     else let sqrtD = sqrt discriminant
          in [ (trace + sqrtD) / 2, (trace - sqrtD) / 2 ]
findEigenvalues2x2 _ = error "2x2 행렬만 지원합니다."

-- 주어진 고윳값에 대한 고유벡터 찾기
-- (A - λI)v = 0 을 만족하는 v를 찾습니다.
-- (a-λ)x + by = 0 에서, 간단한 해는 v = [-b, a-λ] 입니다.
findEigenvector :: MyMatrix -> Double -> MyVector
findEigenvector [[a, b], [c, d]] lambda =
  let v1 = [-b, a - lambda]
      v2 = [d - lambda, -c]
  in -- 더 안정적인 (0이 아닌) 벡터를 선택하고 정규화합니다.
     if vLength v1 > vLength v2 then normalize v1 else normalize v2
findEigenvector _ _ = error "2x2 행렬만 지원합니다."


main :: IO ()
main = do
  -- 변환 행렬 A 정의
  let matrixA = [[3, 1], [1, 2]] :: MyMatrix

  -- 고윳값 계산
  let eigenvalues = findEigenvalues2x2 matrixA
  let lambda1 = head eigenvalues
  let lambda2 = eigenvalues !! 1

  -- 고유벡터 계산
  let eigenvector1 = findEigenvector matrixA lambda1
  let eigenvector2 = findEigenvector matrixA lambda2

  -- 행렬 A를 통해 변환된 고유벡터
  let transformedV1 = matVecMul matrixA eigenvector1
  let transformedV2 = matVecMul matrixA eigenvector2
  
  -- 스케일링된 원본 고유벡터 (비교용)
  let scaledV1 = vScale lambda1 eigenvector1
  let scaledV2 = vScale lambda2 eigenvector2

  putStrLn $ "행렬 A: " ++ show matrixA
  putStrLn "----------------------------------"
  putStrLn $ "고윳값 1 (λ1): " ++ show lambda1
  putStrLn $ "  - 고유벡터 1 (v1): " ++ show eigenvector1
  putStrLn $ "  - A * v1:          " ++ show transformedV1
  putStrLn $ "  - λ1 * v1:         " ++ show scaledV1
  putStrLn "----------------------------------"
  putStrLn $ "고윳값 2 (λ2): " ++ show lambda2
  putStrLn $ "  - 고유벡터 2 (v2): " ++ show eigenvector2
  putStrLn $ "  - A * v2:          " ++ show transformedV2
  putStrLn $ "  - λ2 * v2:         " ++ show scaledV2
  putStrLn "----------------------------------"


  toFile def "eigen_vectors.png" $ do
    layout_title .= "Eigenvectors and Transformation"
    layout_x_axis . laxis_title .= "x"
    layout_y_axis . laxis_title .= "y"
    
    plot (liftEC $ do
      plot_lines_values .= [[(0,0), toTuple eigenvector1]]
      plot_lines_style . line_color .= opaque blue
      plot_lines_style . line_width .= 2.0
      plot_lines_title .= "eigenvector 1"
      )

    plot (liftEC $ do
      plot_lines_values .= [[(0,0), toTuple eigenvector2]]
      plot_lines_style . line_color .= opaque green
      plot_lines_style . line_width .= 2.0
      plot_lines_title .= "eigenvector 2"
      )

    plot (liftEC $ do
      plot_lines_values .= [[(0,0), toTuple transformedV1]]
      plot_lines_style . line_color .= opaque red
      plot_lines_style . line_width .= 1.0
      plot_lines_style . line_dashes .= [10, 5]
      plot_lines_title .= "A * v1"
      )

    plot (liftEC $ do
      plot_lines_values .= [[(0,0), toTuple transformedV2]]
      plot_lines_style . line_color .= opaque orange
      plot_lines_style . line_width .= 1.0
      plot_lines_style . line_dashes .= [10, 5]
      plot_lines_title .= "A * v2"
      )
