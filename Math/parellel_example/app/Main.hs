module Main where

import Control.Parallel          (par, pseq)
import Control.Parallel.Strategies
import Control.Concurrent.Async  (concurrently, mapConcurrently)
import Data.Time.Clock           (getCurrentTime, diffUTCTime)

-- ============================================================
-- 예제 1: par / pseq — 가장 기초적인 병렬 힌트
-- ============================================================
-- par  x y : x를 스파크(spark)로 등록해 다른 코어에서 평가 시도, y 반환
-- pseq x y : x를 완전히 평가한 뒤 y 반환 (순서 보장)

fib :: Int -> Int
fib 0 = 0
fib 1 = 1
fib n = fib (n - 1) + fib (n - 2)

-- 두 fib 계산을 병렬로 실행
parFib :: Int -> Int -> (Int, Int)
parFib a b =
  let ra = fib a
      rb = fib b
  in ra `par` rb `pseq` (ra, rb)   -- ra를 스파크에 올려두고, rb 평가 후 합산

-- ============================================================
-- 예제 2: Strategies — 선언적 병렬화
-- ============================================================
-- parMap rdeepseq f xs : 리스트 각 원소를 병렬로 f 에 적용
-- rdeepseq             : 결과를 완전히 평가(깊은 평가)

heavyCalc :: Int -> Int
heavyCalc n = sum [1..n * 10000]

-- Strategies 기반 병렬 맵
parallelMap :: [Int] -> [Int]
parallelMap xs = parMap rdeepseq heavyCalc xs

-- ============================================================
-- 예제 3: Async — IO 작업 병렬화
-- ============================================================
-- 네트워크 요청, 파일 IO 등 실제 IO를 병렬로 처리할 때 사용

simulateWork :: String -> Int -> IO String
simulateWork name ms = do
  let result = sum [1..ms * 1000]   -- CPU 작업으로 IO 지연 흉내
  return $ name ++ " 완료 (합계: " ++ show result ++ ")"

-- 두 작업을 동시에 실행
asyncExample :: IO ()
asyncExample = do
  putStrLn "\n[Async] 두 작업 병렬 실행"
  (r1, r2) <- concurrently
    (simulateWork "작업-A" 300)
    (simulateWork "작업-B" 500)
  putStrLn r1
  putStrLn r2

-- 리스트의 모든 작업을 병렬로
asyncMapExample :: IO ()
asyncMapExample = do
  putStrLn "\n[Async] 리스트 병렬 처리"
  let tasks = [("서비스-" ++ show i, i * 100) | i <- [1..5]]
  results <- mapConcurrently (uncurry simulateWork) tasks
  mapM_ putStrLn results

-- ============================================================
-- 유틸: 실행 시간 측정
-- ============================================================
timed :: String -> IO a -> IO a
timed label action = do
  start  <- getCurrentTime
  result <- action
  end    <- getCurrentTime
  let elapsed = diffUTCTime end start
  putStrLn $ label ++ " → " ++ show elapsed
  return result

-- ============================================================
-- main
-- ============================================================
main :: IO ()
main = do
  putStrLn "=== 하스켈 병렬 처리 예제 ==="

  -- 예제 1: par / pseq
  putStrLn "\n[1] par/pseq — 기초 병렬 힌트"
  timed "fib 35 + fib 36 (병렬)" $ do
    let (a, b) = parFib 35 36
    putStrLn $ "fib 35 = " ++ show a ++ ", fib 36 = " ++ show b

  -- 예제 2: Strategies
  putStrLn "\n[2] Strategies — parMap"
  timed "parMap heavyCalc [1..8]" $ do
    let results = parallelMap [1..8]
    putStrLn $ "결과 개수: " ++ show (length results)
    putStrLn $ "첫 번째: " ++ show (head results)

  -- 예제 3: Async
  timed "Async 예제" asyncExample
  timed "Async 맵 예제" asyncMapExample

  putStrLn "\n완료"
