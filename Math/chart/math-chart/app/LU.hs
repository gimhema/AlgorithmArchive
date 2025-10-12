-- file: Main.hs
-- build: ghc -O2 Main.hs && ./Main
-- 기본 행렬 연산으로 구현한 LU 분해 (부분 피벗팅, 방정식 풀이 없음)

module Main where

import Data.List (transpose, maximumBy)
import Data.Ord  (comparing)
import Numeric   (showFFloat)

type Matrix = [[Double]]

--------------------------------------------------------------------------------
-- 기본 행렬 연산
--------------------------------------------------------------------------------
nRows :: Matrix -> Int
nRows = length

nCols :: Matrix -> Int
nCols []     = 0
nCols (r:_)  = length r

isRectangular :: Matrix -> Bool
isRectangular []     = True
isRectangular (r:rs) = all ((== length r) . length) rs

identity :: Int -> Matrix
identity n = [ [ if i==j then 1 else 0 | j <- [0..n-1] ] | i <- [0..n-1] ]

transposeM :: Matrix -> Matrix
transposeM = transpose

matMul :: Matrix -> Matrix -> Matrix
matMul a b
  | nCols a /= nRows b = error "matMul: dimension mismatch"
  | otherwise =
      let bt = transposeM b
      in [ [ sum (zipWith (*) row col) | col <- bt ] | row <- a ]

replaceAt :: Int -> a -> [a] -> [a]
replaceAt i v xs = [ if k==i then v else xs !! k | k <- [0..length xs - 1] ]

-- 보기 좋은 출력
pretty :: Matrix -> String
pretty = unlines . map (unwords . map fmt)
  where
    fmt x =
      let r = round x :: Integer
      in if abs (x - fromIntegral r) < 1e-12
           then show r
           else trim (showFFloat (Just 6) x "")
    trim s =
      case break (=='.') s of
        (i,"")     -> i
        (i,'.':fs) ->
          let fs' = reverse (dropWhile (=='0') (reverse fs))
          in if null fs' then i else i ++ "." ++ fs'

--------------------------------------------------------------------------------
-- 순열행렬, 소거행렬, L 갱신
--------------------------------------------------------------------------------
-- k <-> r 행을 바꾸는 순열행렬 P(k,r)
permSwap :: Int -> Int -> Int -> Matrix
permSwap n k r
  | k==r      = identity n
  | otherwise = [ [ if j==idx i then 1 else 0 | j <- [0..n-1] ] | i <- [0..n-1] ]
  where
    idx i | i==k = r
          | i==r = k
          | otherwise = i

-- k단계 소거행렬 E_k: U' = E_k * U (대각 1, i>k에 대해 E[i,k] = -m_i)
elimMatrix :: Int -> Int -> [Double] -> Matrix
elimMatrix n k multipliers =
  [ [ entry i j | j <- [0..n-1] ] | i <- [0..n-1] ]
  where
    entry i j
      | i == j                     = 1
      | i > k && j == k            = -(multipliers !! (i - (k+1))) -- i=k+1..n-1
      | otherwise                  = 0

-- L의 k열에 곱셈자 m_i 기록 (대각 1)
updateL :: Matrix -> Int -> [Double] -> Matrix
updateL l k multipliers =
  [ if i == k
      then replaceAt k 1 (l !! i)
      else if i > k
        then replaceAt k (multipliers !! (i - (k+1))) (l !! i)
        else l !! i
  | i <- [0..n-1] ]
  where n = nRows l

-- L의 “이미 확정된 부분(0..k-1열)”만 행 스왑 적용
swapRowsInLPrefix :: Matrix -> Int -> Int -> Int -> Matrix
swapRowsInLPrefix l k r colFixed =
  [ if i==k then rowR
    else if i==r then rowK
    else l !! i
  | i <- [0..n-1] ]
  where
    n     = nRows l
    rowK  = take colFixed (l !! r) ++ drop colFixed (l !! k)
    rowR  = take colFixed (l !! k) ++ drop colFixed (l !! r)

--------------------------------------------------------------------------------
-- LU 분해 (기본행렬 사용, 부분 피벗팅)
--------------------------------------------------------------------------------
-- 반환: (P, L, U) such that P*A = L*U
luDecomposeBasic :: Matrix -> Either String (Matrix, Matrix, Matrix)
luDecomposeBasic a
  | not (isRectangular a)      = Left "LU: 입력 행렬은 직사각형이어야 합니다."
  | nRows a /= nCols a         = Left "LU: 정방행렬만 분해할 수 있습니다."
  | otherwise                  = Right (pFinal, lFinal, uFinal)
  where
    n = nRows a
    p0 = identity n
    l0 = replicate n (replicate n 0)  -- 나중에 대각 1로 채워짐
    u0 = a

    (pFinal, lFinal, uFinal) = go 0 p0 l0 u0

    go :: Int -> Matrix -> Matrix -> Matrix -> (Matrix, Matrix, Matrix)
    go k p l u
      | k == n    = (p, l, u)
      | otherwise =
          -- 1) 피벗 선택 (부분 피벗팅)
          let (r, pivotAbs) =
                maximumBy (comparing snd)
                  [ (i, abs ((u !! i) !! k)) | i <- [k..n-1] ]
          in if pivotAbs < 1e-15
               then error "LU: singular or nearly singular pivot encountered."
               else
                 -- 2) 순열행렬로 U, P 스왑 적용
                 let pk     = permSwap n k r
                     u'     = matMul pk u
                     p'     = matMul pk p
                     -- L에는 지금까지 결정된 컬럼(0..k-1)까지만 행 스왑을 반영
                     l1     = swapRowsInLPrefix l k r k
                     ukk    = (u' !! k) !! k
                     -- 3) 곱셈자(multipliers) 계산 (i=k+1..n-1)
                     ms     = [ (u' !! i) !! k / ukk | i <- [k+1..n-1] ]
                     -- 4) 소거행렬 E_k 구성, U 갱신: U_next = E_k * U'
                     ek     = elimMatrix n k ms
                     uNext  = matMul ek u'
                     -- 5) L에 곱셈자 기록 (대각 1)
                     lNext  = updateL l1 k ms
                 in go (k+1) p' lNext uNext

--------------------------------------------------------------------------------
-- 데모: 분해와 검증 출력만 (방정식 풀이 없음)
--------------------------------------------------------------------------------
main :: IO ()
main = do
  let a = [ [1, 2, 3]
           , [2, -4, 6]
           , [3, -9, -3]
           ]

  putStrLn "입력 행렬 A:"
  putStrLn (pretty a)

  case luDecomposeBasic a of
    Left err    -> putStrLn ("LU 실패: " ++ err)
    Right (p,l,u) -> do
      putStrLn "P:"
      putStrLn (pretty p)
      putStrLn "L:"
      putStrLn (pretty l)
      putStrLn "U:"
      putStrLn (pretty u)

      let lhs = matMul p a
          rhs = matMul l u
      putStrLn "검증: P*A 와 L*U"
      putStrLn "P*A:"
      putStrLn (pretty lhs)
      putStrLn "L*U:"
      putStrLn (pretty rhs)
