# Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .
set terminal svg background rgb "white" size 900,700
set output "plot_results.svg"

set multiplot layout 2,1 title "Artificial Neural Network Performance" font ",16"

# Top Plot: Function Interpolation & Calculus
set title "Function Interpolation & Analytical Calculus Fields"
set xlabel "x"
set ylabel "y"
set grid
set key outside right
plot "interpolation.txt" using 1:2 with lines lw 2 lc rgb "black" title "Exact g(x)", \
     "interpolation.txt" using 1:3 with points pt 7 ps 0.6 lc rgb "red" title "ANN Fit F_p(x)", \
     "interpolation.txt" using 1:4 with lines lw 1.5 lc rgb "blue" title "ANN F'_p(x)", \
     "interpolation.txt" using 1:5 with lines lw 1.5 lc rgb "magenta" title "ANN F''_p(x)", \
     "interpolation.txt" using 1:6 with lines lw 1.5 lc rgb "dark-green" title "ANN Anti-deriv"

# Bottom Plot: ODE Solver
set title "ODE Solution (y'' + y = 0, y(0)=1, y'(0)=0)"
set xlabel "x"
set ylabel "y(x)"
set grid
set key outside right
plot "ode_solution.txt" using 1:2 with lines lw 2 lc rgb "black" title "Exact (cos x)", \
     "ode_solution.txt" using 1:3 with points pt 5 ps 0.8 lc rgb "orange-red" title "ANN ODE Solver"

unset multiplot
