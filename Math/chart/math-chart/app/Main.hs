import Graphics.Rendering.Chart.Easy
import Graphics.Rendering.Chart.Backend.Diagrams
import Data.Default.Class (def)

main :: IO ()
main = toFile def "vector_plot.png" (myPlot :: EC (Layout Double Double) ())
  where
    myPlot = do
        layout_title .= "Vector Visualization"
        plot (line "v1" [ [(0.0, 0.0), (1.0, 2.0)] ])
        plot (line "v2" [ [(0.0, 0.0), (-1.0, 1.0)] ])
