# Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .
set terminal svg background rgb "white" size 900,900
set output "splines_results.svg"
set multiplot layout 3,1 title "Spline Interpolations (Data: cos(x))" font ",14"

# 1. Linear Spline
set title "Linear Spline and Integral"
set key top right
plot "data.txt" using 1:2 with points pt 7 ps 1.5 lc rgb "black" title "Data points", \
     "plot_curves.txt" using 1:2 with lines lw 2 lc rgb "blue" title "Linear Interpolant", \
     "plot_curves.txt" using 1:3 with lines dt 2 lw 2 lc rgb "red" title "Calculated Integral", \
     "plot_curves.txt" using 1:7 with lines dt 4 lw 2 lc rgb "green" title "Analytical Integral (sin(x))"

# 2. Quadratic Spline
set title "Quadratic Spline"
plot "data.txt" using 1:2 with points pt 7 ps 1.5 lc rgb "black" title "Data points", \
     "plot_curves.txt" using 1:4 with lines lw 2 lc rgb "purple" title "Quadratic Interpolant"

# 3. Cubic Spline
set title "Cubic Spline"
plot "data.txt" using 1:2 with points pt 7 ps 1.5 lc rgb "black" title "Data points", \
     "plot_curves.txt" using 1:6 with lines lw 2 lc rgb "orange" title "Cubic Interpolant"

unset multiplot
