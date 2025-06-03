import Graphics.Rendering.Chart.Easy
import Graphics.Rendering.Chart.Backend.Diagrams
import Data.Default.Class (def)

type MyVector = [Double]
type MyMatrix = [[Double]]

-- 벡터 연산
vAdd :: MyVector -> MyVector -> MyVector
vAdd = zipWith (+)

vScale :: Double -> MyVector -> MyVector
vScale a = map (a *)

-- 행렬-벡터 곱
matVecMul :: MyMatrix -> MyVector -> MyVector
matVecMul m v
  | all ((== length v) . length) m = map (dot v) m
  | otherwise = error "Incompatible dimensions for matrix-vector multiplication"
  where
    dot :: MyVector -> [Double] -> Double
    dot xs ys = sum $ zipWith (*) xs ys

-- 행렬-행렬 곱
matMul :: MyMatrix -> MyMatrix -> MyMatrix
matMul a b
  | null a || null b = error "Empty matrix"
  | length (head a) /= length b = error "Incompatible matrix dimensions for multiplication"
  | otherwise = [ [ sum $ zipWith (*) row col | col <- transpose b ] | row <- a ]
  where
    transpose :: MyMatrix -> MyMatrix
    transpose ([]:_) = []
    transpose x = map head x : transpose (map tail x)

plotVec2 :: MyVector -> EC l (PlotLines Double Double)
plotVec2 [x, y] = line "" [[(0, 0), (x, y)]]
plotVec2 _ = error "Only 2D vectors can be plotted"


main :: IO ()
main = do
  -- 2D 벡터 시각화 예제
  toFile def "matrix_vector_multiplication_2d.png" $ do
    layout_title .= "Matrix-Vector Multiplication (2D)"

    let v2 = [1.0, 2.0]
    let m2 = [ [2.0, 1.0]
             , [0.0, 1.0] ]
    let result2 = matVecMul m2 v2

    plot (line "v"       [ [(0,0), (v2 !! 0, v2 !! 1)] ])
    plot (line "M * v"   [ [(0,0), (result2 !! 0, result2 !! 1)] ])

  -- 3D 행렬 * 벡터 예제
  let v3 = [1.0, 2.0, 3.0]
  let m3 = [ [1.0, 0.0, 2.0]
           , [0.0, 1.0, 0.0]
           , [0.0, 0.0, 1.0] ]
  let result3 = matVecMul m3 v3

  putStrLn "=== 3D Matrix-Vector Multiplication ==="
  putStrLn $ "Original vector:      " ++ show v3
  putStrLn $ "Transformed vector:   " ++ show result3

  -- 행렬 * 행렬 예제
  let a = [ [1, 2, 3]
          , [4, 5, 6] ]
  let b = [ [7,  8]
          , [9, 10]
          , [11, 12] ]
  let c = matMul a b

  putStrLn "\n=== Matrix-Matrix Multiplication ==="
  putStrLn "Matrix A:"
  mapM_ print a
  putStrLn "Matrix B:"
  mapM_ print b
  putStrLn "Result A * B:"
  mapM_ print c
