{-# LANGUAGE TupleSections #-}
module Main where

import Data.List (transpose, maximumBy)
import Data.Ord  (comparing)
import Control.Monad (foldM)

-- 간단한 행렬/벡터 타입
type Matrix = [[Double]]
type Vector = [Double]

------------------------
-- 유틸리티
------------------------
eye :: Int -> Matrix
eye n = [ [ if i==j then 1 else 0 | j <- [0..n-1] ] | i <- [0..n-1] ]

dims :: Matrix -> (Int, Int)
dims m = (length m, if null m then 0 else length (head m))

matMul :: Matrix -> Matrix -> Matrix
matMul a b =
  let (_, p) = dims a
      (p', q) = dims b
  in if p /= p' then error "matMul: incompatible shapes"
     else [ [ sum $ zipWith (*) ar bc | bc <- transpose b ] | ar <- a ]

matVec :: Matrix -> Vector -> Vector
matVec a v = [ sum $ zipWith (*) r v | r <- a ]

augment :: Matrix -> Vector -> Matrix
augment a b =
  let (n, m) = dims a
  in if length b /= n then error "augment: incompatible shapes"
     else zipWith (\row x -> row ++ [x]) a b

splitAugment :: Matrix -> (Matrix, Vector)
splitAugment ab =
  let (n, m') = dims ab
      m = m' - 1
      a = map (take m) ab
      b = map last ab
  in (a,b)

pretty :: Matrix -> String
pretty = unlines . map (unwords . map (pad . show))
  where pad s = let k = 12 - length s in replicate (max 1 k) ' ' ++ s

------------------------
-- 기본행렬(초등행렬)
------------------------
-- 1) 행 교환: swap i j
eSwap :: Int -> Int -> Int -> Matrix
eSwap n i j
  | i == j    = eye n
  | otherwise = [ row r | r <- [0..n-1] ]
  where row r
          | r == i    = unit j
          | r == j    = unit i
          | otherwise = unit r
        unit k = [ if c==k then 1 else 0 | c <- [0..n-1] ]

-- 2) 행 스케일: row i <- s * row i
eScale :: Int -> Int -> Double -> Matrix
eScale n i s =
  [ [ if r==i && c==i then s
      else if r==c then 1
      else 0
    | c <- [0..n-1] ]
  | r <- [0..n-1] ]

-- 3) 행 덧셈: row i <- row i + s * row j  (i ≠ j)
eAdd :: Int -> Int -> Int -> Double -> Matrix
eAdd n i j s =
  [ [ base r c + extra r c | c <- [0..n-1] ] | r <- [0..n-1] ]
  where base r c  = if r==c then 1 else 0
        extra r c = if r==i && c==j then s else 0


-- 한 단계: k번째 열을 피벗 1, 다른 행 0으로 만드는 E들을 만들어 누적 적용
step :: (Matrix, Matrix) -> Int -> Either String (Matrix, Matrix)
step (a, accE) k =
  let (n, m) = dims a
  in if k >= n || k >= m then Right (a, accE) else do
      -- 피벗 후보 선택(절댓값 최대 행) - k행 이하에서
      let candidates = [ (r, abs (a !! r !! k)) | r <- [k..n-1] ]
          (pivotRow, pivAbs) =
            if null candidates then (k, 0) else maximumBy (comparing snd) candidates
      if pivAbs == 0
        then Left $ "Singular or rank-deficient at column " ++ show k
        else do
          -- 1) 필요시 행 교환
          let eswapM = if pivotRow /= k then eSwap n k pivotRow else eye n
              a1     = eswapM `matMul` a
              accE1  = eswapM `matMul` accE
          let piv    = a1 !! k !! k
          -- 2) 피벗을 1로 스케일
          let escaleM = eScale n k (1 / piv)
              a2      = escaleM `matMul` a1
              accE2   = escaleM `matMul` accE1
          -- 3) 다른 행들 0으로 만들기
          let elim r mat =
                let c = mat !! r !! k
                in if r == k || c == 0 then (mat, eye n)
                   else let e = eAdd n r k (-c)
                        in (e `matMul` mat, e)
              eliminateAll (mat, eProd) r =
                let (mat', e') = elim r mat
                in (mat', e' `matMul` eProd)
          let (a3, elimProd) = foldl eliminateAll (a2, eye n) [0..n-1]
              accE3          = elimProd `matMul` accE2
          Right (a3, accE3)


-- 전체 RREF: E*A = R(=I) 가 되도록 E들을 누적(accE)해서 반환
rrefWithE :: Matrix -> Either String (Matrix, Matrix)
rrefWithE a =
  let (n, m) = dims a
      steps = [0 .. min n m - 1]
  in foldM step (a, eye n) steps

-- Ax=b 해 풀기: (E*A = I)라면 x = E*b
solveByElementary :: Matrix -> Vector -> Either String Vector
solveByElementary a b = do
  let (n, m) = dims a
  if n /= m then Left "A must be square."
  else do
    (_, e) <- rrefWithE a  -- 여기서 e = A^{-1} (성공 시)
    pure (e `matVec` b)

------------------------
-- 데모
------------------------
-- 예제: 3x3
--  2x +  y -  z =  8
-- -3x - y + 2z = -11
-- -2x + y + 2z = -3
aEx :: Matrix
aEx =
  [ [ 2,  1, -1]
  , [-3, -1,  2]
  , [-2,  1,  2]
  ]

bEx :: Vector
bEx = [8, -11, -3]

main :: IO ()
main = do
  putStrLn "A ="
  putStrLn (pretty aEx)
  putStrLn "b ="
  print bEx
  putStrLn ""

  case rrefWithE aEx of
    Left err -> putStrLn $ "RREF failed: " ++ err
    Right (r, e) -> do
      putStrLn "R = E * A (RREF of A, ideally identity):"
      putStrLn (pretty r)
      putStrLn "E (product of elementary matrices, equals A^{-1} if A is nonsingular):"
      putStrLn (pretty e)

  putStrLn "Solving A x = b using elementary matrices..."
  case solveByElementary aEx bEx of
    Left err -> putStrLn $ "Solve failed: " ++ err
    Right x  -> do
      putStrLn "x (rounded) ="
      print (map round x)
      -- 검증: A*x
      let bCheck = aEx `matVec` x
      putStrLn "A * x (should equal b) ="
      print (map round bCheck)
      -- print bCheck
