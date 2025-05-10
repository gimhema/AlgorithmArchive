module Main where

-- 함수 정의와 타입 시그니처
add :: Int -> Int -> Int
add x y = x + y

-- 조건문 if
isEven :: Int -> Bool
isEven x = if x `mod` 2 == 0 then True else False

-- 패턴 매칭
factorial :: Int -> Int
factorial 0 = 1
factorial n = n * factorial (n - 1)

-- 리스트와 리스트 함수
numbers :: [Int]
numbers = [1..5]

doubleList :: [Int] -> [Int]
doubleList xs = map (*2) xs

-- 리스트 패턴 매칭
describeList :: [a] -> String
describeList [] = "Empty list"
describeList [x] = "Single element"
describeList (x:y:_) = "List with at least two elements"

-- 튜플
sumAndProduct :: Int -> Int -> (Int, Int)
sumAndProduct x y = (x + y, x * y)

-- 고차 함수
applyTwice :: (a -> a) -> a -> a
applyTwice f x = f (f x)

-- 메인 함수 (입출력)
main :: IO ()
main = do
  putStrLn "=== Basic Haskell Syntax ==="

  -- 변수와 함수 사용
  let a = 3
  let b = 4
  putStrLn ("Add: " ++ show (add a b))

  -- 조건문
  putStrLn ("Is " ++ show a ++ " even? " ++ show (isEven a))

  -- 재귀
  putStrLn ("Factorial of 5: " ++ show (factorial 5))

  -- 리스트 다루기
  putStrLn ("Original list: " ++ show numbers)
  putStrLn ("Doubled list: " ++ show (doubleList numbers))
  putStrLn ("List description: " ++ describeList numbers)

  -- 튜플
  let (s, p) = sumAndProduct a b
  putStrLn ("Sum: " ++ show s ++ ", Product: " ++ show p)

  -- 고차 함수
  putStrLn ("Apply (*2) twice to 3: " ++ show (applyTwice (*2) 3))
