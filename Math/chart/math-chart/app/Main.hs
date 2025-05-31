import Graphics.Rendering.Chart.Easy
import Graphics.Rendering.Chart.Backend.Diagrams
import Data.Default.Class (def)

type Vec2 = (Double, Double)
type Mat2x2 = ((Double, Double), (Double, Double))  -- ((a, b), (c, d))

-- 벡터 연산
vAdd :: Vec2 -> Vec2 -> Vec2
vAdd (x1, y1) (x2, y2) = (x1 + x2, y1 + y2)

vScale :: Double -> Vec2 -> Vec2
vScale a (x, y) = (a * x, a * y)

-- 행렬-벡터 곱: M * v
matVecMul :: Mat2x2 -> Vec2 -> Vec2
matVecMul ((a, b), (c, d)) (x, y) = (a*x + b*y, c*x + d*y)

main :: IO ()
main = toFile def "matrix_vector_multiplication.png" $ do
    layout_title .= "Matrix-Vector Multiplication"

    -- 원래 벡터와 행렬 정의
    let v = (1.0, 2.0)
    let m = ((2.0, 1.0), (0.0, 1.0))  -- 2x2 행렬

    -- 행렬 곱 결과
    let v_transformed = matVecMul m v

    -- 시각화
    plot (line "v"              [ [(0,0), v] ])
    plot (line "M * v"          [ [(0,0), v_transformed] ])
