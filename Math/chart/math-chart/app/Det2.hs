{-# LANGUAGE TemplateHaskell #-}

import Graphics.Rendering.Chart.Easy
import Graphics.Rendering.Chart.Backend.Diagrams
import Data.Default.Class (def)

type MyVector = [Double]

-- 벡터 연산
vAdd :: MyVector -> MyVector -> MyVector
vAdd [x1, y1] [x2, y2] = [x1 + x2, y1 + y2]
vAdd _ _ = error "Only 2D vectors supported"

toTuple :: MyVector -> (Double, Double)
toTuple [x, y] = (x, y)
toTuple _ = error "Only 2D vectors supported"

-- 평행사변형 넓이 = |det|
parallelogramArea :: MyVector -> MyVector -> Double
parallelogramArea [x1, y1] [x2, y2] = abs (x1 * y2 - x2 * y1)
parallelogramArea _ _ = error "Only 2D vectors supported"

main :: IO ()
main = do
  let v1 = [3, 1]
  let v2 = [2, 4]
  let v3 = vAdd v1 v2
  let area = parallelogramArea v1 v2

  putStrLn $ "벡터 v1: " ++ show v1
  putStrLn $ "벡터 v2: " ++ show v2
  putStrLn $ "평행사변형 넓이 (|det|): " ++ show area

  -- 꼭짓점 4개를 연결하는 다각형
  let poly = [ (0,0)
             , toTuple v1
             , toTuple v3
             , toTuple v2
             , (0,0) ]

  toFile def "parallelogram_filled.png" $ do
    layout_title .= "Parallelogram Area (Filled)"
    setColors [opaque lightblue, opaque red, opaque green]
    -- 색채운 다각형: 폐곡선 하나로 표현
    plot (line "area" [poly])
    -- 벡터 v1
    plot (line "v1" [[(0,0), toTuple v1]])
    -- 벡터 v2
    plot (line "v2" [[(0,0), toTuple v2]])
