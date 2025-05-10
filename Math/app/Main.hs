module Main where

-- Functions
add :: Int -> Int -> Int
add x y = x + y

mul :: Int -> Int -> Int
mul x y = x * y

main :: IO ()
main = do
  let add_result = add 1 2
  let mul_result = mul 3 5
  putStrLn ("Add Result : " ++ show add_result)
  putStrLn ("Mul Result : " ++ show mul_result)
