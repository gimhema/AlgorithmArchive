{-# LANGUAGE TemplateHaskell #-}

-- Row-space Subspace Check + PNG Plot
-- ghc -O2 app/Main.hs -o rowspace && ./rowspace
-- 결과 이미지: rowspace_subspace.png

import Graphics.Rendering.Chart.Easy hiding (Vector, Matrix)
import Graphics.Rendering.Chart.Backend.Diagrams
import Data.Default.Class (def)
import Data.List (maximumBy)
import Data.Ord  (comparing)

-- ====== 타입 ======
type Vector = [Double]
type Matrix = [[Double]]

eps :: Double
eps = 1e-9

-- ====== 유틸 ======
nCols :: Matrix -> Int
nCols []    = 0
nCols (r:_) = length r

sameLengthRows :: Matrix -> Bool
sameLengthRows []     = True
sameLengthRows (r:rs) = all ((== length r) . length) rs

dimCheckMatrix :: Matrix -> Either String Matrix
dimCheckMatrix m
  | not (sameLengthRows m) = Left "행렬의 각 행 길이가 다릅니다."
  | otherwise              = Right m

dimCheckAppend :: Matrix -> Matrix -> Either String Matrix
dimCheckAppend a b
  | all ((== nCols a) . length) b = Right (a ++ b)
  | otherwise = Left "붙이려는 행들의 길이가 A의 열 수와 다릅니다."

isZeroRow :: Vector -> Bool
isZeroRow = all (\x -> abs x <= eps)

-- ====== RREF (부분 피벗팅) ======
rref :: Matrix -> Matrix
rref m0 =
  let Right m = dimCheckMatrix m0
      rows = length m
      cols = nCols m
   in go m 0 0 rows cols
  where
    go :: Matrix -> Int -> Int -> Int -> Int -> Matrix
    go m i j r c
      | i >= r || j >= c = m
      | otherwise =
          case pivotRowIndex m i j r of
            Nothing   -> go m i (j+1) r c
            Just pidx ->
              let m1  = swapRows m i pidx
                  piv = (m1 !! i) !! j
                  m2  = scaleRow m1 i (1 / piv)
                  m3  = eliminateCol m2 i j
              in go m3 (i+1) (j+1) r c

    pivotRowIndex :: Matrix -> Int -> Int -> Int -> Maybe Int
    pivotRowIndex m i j r =
      let cs = [k | k <- [i..r-1], abs ((m !! k) !! j) > eps]
      in if null cs then Nothing
         else Just $ maximumBy (comparing (\k -> abs ((m !! k) !! j))) cs

    swapRows :: Matrix -> Int -> Int -> Matrix
    swapRows m a b
      | a == b    = m
      | otherwise =
          let ra = m !! a; rb = m !! b
          in [ if idx == a then rb
               else if idx == b then ra
               else row
             | (row, idx) <- zip m [0..] ]

    scaleRow :: Matrix -> Int -> Double -> Matrix
    scaleRow m idx s =
      [ if i == idx then map (* s) row else row
      | (row, i) <- zip m [0..] ]

    eliminateCol :: Matrix -> Int -> Int -> Matrix
    eliminateCol m pivRow pivCol =
      [ if r == pivRow then m !! r
        else
          let coeff = (m !! r) !! pivCol
              newRow = zipWith (\a b -> a - coeff * b) (m !! r) (m !! pivRow)
          in newRow
      | r <- [0..length m - 1] ]

-- 랭크 = RREF에서 0 아닌 행 개수
rank :: Matrix -> Int
rank = length . filter (not . isZeroRow) . rref

-- ====== 포함성 판정 ======
-- 단일 벡터 v ∈ Row(A)?  <=>  rank([A; v]) == rank(A)
inRowSpace :: Matrix -> Vector -> Either String Bool
inRowSpace a v = do
  _ <- dimCheckMatrix a
  if null a
    then Left "행렬 A가 비어 있습니다."
    else if length v /= nCols a
          then Left "벡터 v의 길이가 A의 열 수와 다릅니다."
          else do
            let rA = rank a
            av <- dimCheckAppend a [v]
            let rAV = rank av
            pure (rAV == rA)

-- 여러 벡터 집합 W ⊆ Row(A)?  <=>  rank([A; W]) == rank(A)
inRowSpaceSet :: Matrix -> [Vector] -> Either String Bool
inRowSpaceSet a ws = do
  _ <- dimCheckMatrix a
  if any ((/= nCols a) . length) ws
    then Left "W의 어떤 벡터는 A의 열 수와 다릅니다."
    else do
      let rA = rank a
      aw <- dimCheckAppend a ws
      pure (rank aw == rA)

-- ====== 2D 시각화 도우미 ======
toTuple :: Vector -> (Double, Double)
toTuple [x,y] = (x,y)
toTuple _     = error "2D 전용 도우미입니다."

arrowFromOrigin :: Vector -> [(Double, Double)]
arrowFromOrigin v = [(0,0), toTuple v]

spanLine :: Vector -> Double -> [(Double, Double)]
spanLine dir len =
  let [x,y]   = dir
      nrm     = sqrt (x*x + y*y)
      (ux,uy) = if nrm < eps then (1,0) else (x/nrm, y/nrm)
  in [(-len*ux, -len*uy), (len*ux, len*uy)]

-- ====== 예제 입력 (2×2) ======
-- A는 rank 1: Row(A)는 직선 하나
a2 :: Matrix
a2 =
  [ [1, 2]
  , [2, 4]
  ]

rowBasisDir :: Vector
rowBasisDir = head a2  -- [1,2] 방향

-- 검사할 벡터들
w1, w2, u :: Vector
w1 = [2, 4]    -- 포함
w2 = [-0.5,-1] -- 포함
u  = [1, 0]    -- 불포함

prettyMatrix :: Matrix -> String
prettyMatrix = unlines . map show

main :: IO ()
main = do
  putStrLn "행렬 A:"
  putStrLn (prettyMatrix a2)
  let rA = rank a2
  putStrLn $ "rank(A) = " ++ show rA
  putStrLn "RREF(A):"
  print (rref a2)

  let chk v name = do
        case inRowSpace a2 v of
          Left err  -> putStrLn $ name ++ " -> 오류: " ++ err
          Right res -> putStrLn $ name ++ " ∈ Row(A) ? " ++ show res

  chk w1 "w1"
  chk w2 "w2"
  chk u  "u "

  -- === PNG 시각화 ===
  let axisLen = 5
      lineLen = 4

  toFile def "rowspace_subspace.png" $ do
    layout_title .= "Row Space Subspace Check (2D)"
    layout_x_axis . laxis_title .= "x"
    layout_y_axis . laxis_title .= "y"
    layout_x_axis . laxis_generate .= scaledAxis def (-axisLen, axisLen)
    layout_y_axis . laxis_generate .= scaledAxis def (-axisLen, axisLen)

    -- Row(A) 직선 (파랑)
    plot (liftEC $ do
      plot_lines_values .= [spanLine rowBasisDir lineLen]
      plot_lines_style  . line_color .= opaque blue
      plot_lines_style  . line_width .= 2.0
      plot_lines_title  .= "Row(A) (rank 1)"
      )

    -- w1 (초록: 포함)
    plot (liftEC $ do
      plot_lines_values .= [arrowFromOrigin w1]
      plot_lines_style  . line_color .= opaque green
      plot_lines_style  . line_width .= 2.0
      plot_lines_title  .= "w1 (in)"
      )

    -- w2 (초록: 포함)
    plot (liftEC $ do
      plot_lines_values .= [arrowFromOrigin w2]
      plot_lines_style  . line_color .= opaque darkgreen
      plot_lines_style  . line_width .= 2.0
      plot_lines_title  .= "w2 (in)"
      )

    -- u (빨강: 불포함)
    plot (liftEC $ do
      plot_lines_values .= [arrowFromOrigin u]  
      plot_lines_style  . line_color .= opaque red
      plot_lines_style  . line_width .= 2.0
      plot_lines_title  .= "u (out)"
      )
