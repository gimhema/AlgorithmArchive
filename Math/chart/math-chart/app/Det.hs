import Graphics.Rendering.Chart.Easy
import Graphics.Rendering.Chart.Backend.Diagrams
import Data.Default.Class (def)

type MyMatrix = [[Double]]

-- 2x2 행렬식
det2x2 :: MyMatrix -> Double
det2x2 [[a, b], [c, d]] = a * d - b * c
det2x2 _ = error "Not a 2x2 matrix"

-- 3x3 행렬식
det3x3 :: MyMatrix -> Double
det3x3 [[a,b,c], [d,e,f], [g,h,i]] =
  a * (e * i - f * h) -
  b * (d * i - f * g) +
  c * (d * h - e * g)
det3x3 _ = error "Not a 3x3 matrix"


-- 주어진 행과 열을 제거한 소행렬
minorMatrix :: Int -> Int -> MyMatrix -> MyMatrix
minorMatrix i j m = [ [ x | (x, cj) <- zip row [0..], cj /= j ]
                    | (row, ci) <- zip m [0..], ci /= i ]

main :: IO ()
main = do
  putStrLn "=== Determinant Examples ==="

  let mat2x2 = [[3, 8], [4, 6]]
  let mat3x3 = [[6, 1, 1],
                [4, -2, 5],
                [2, 8, 7]]

  putStrLn "\n2x2 Matrix:"
  mapM_ print mat2x2
  putStrLn $ "Determinant (2x2): " ++ show (det2x2 mat2x2)

  putStrLn "\n3x3 Matrix:"
  mapM_ print mat3x3
  putStrLn $ "Determinant (3x3): " ++ show (det3x3 mat3x3)
