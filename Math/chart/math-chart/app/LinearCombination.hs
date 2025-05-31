import Graphics.Rendering.Chart.Easy
import Graphics.Rendering.Chart.Backend.Diagrams
import Data.Default.Class (def)

type Vec2 = (Double, Double)

-- 벡터 연산 함수
vAdd :: Vec2 -> Vec2 -> Vec2
vAdd (x1, y1) (x2, y2) = (x1 + x2, y1 + y2)

vScale :: Double -> Vec2 -> Vec2
vScale a (x, y) = (a*x, a*y)

main :: IO ()
main = toFile def "vector_operations.png" $ do
    layout_title .= "Vector Operations: Scalar Multiplication, Addition, Linear Combination"

    -- 원래 벡터
    let v1 = (1.0, 2.0)
    let v2 = (-1.0, 1.0)

    -- 벡터 연산
    let v1_scaled = vScale 2 v1        -- 2 * v1
    let v_sum = vAdd v1 v2             -- v1 + v2
    let lin_comb = vAdd (vScale 1 v1) (vScale 2 v2)  -- v1 + 2 * v2

    -- 벡터 시각화
    plot (line "v1"        [ [(0,0), v1] ])
    plot (line "v2"        [ [(0,0), v2] ])
    plot (line "2 * v1"    [ [(0,0), v1_scaled] ])
    plot (line "v1 + v2"   [ [(0,0), v_sum] ])
    plot (line "v1 + 2*v2" [ [(0,0), lin_comb] ])
