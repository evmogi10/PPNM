# Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .
set terminal svg background rgb "white" size 800,800
set output "integration_results.svg"
set multiplot layout 2,1 title "Integration Homework Results" font ",16"

# --- Part 1: Error Function Plot ---
set title "Error Function: erf(z)"
set xlabel "z"
set ylabel "erf(z)"
set key top left
plot "erf_plot.txt" using 1:2 with points pt 7 ps 0.8 lc rgb "red" title "Calculated", \
     "erf_plot.txt" using 1:3 with lines lw 2 lc rgb "blue" title "Tabulated (math.h)"

# --- Part 2: Convergence Plot (log-log) ---
set title "Error of erf(1) vs Accuracy Goal (acc)"
set xlabel "Requested Accuracy (acc)"
set ylabel "Actual Absolute Error"
set logscale x 10
set logscale y 10
set format x "10^{%L}"
set format y "10^{%L}"
set grid
set key top left
# Plot the data and a reference line y=x
plot "erf_conv.txt" using 1:2 with linespoints pt 7 lw 2 lc rgb "dark-green" title "Actual Error |Calculated - Exact|", \
     x with lines dt 2 lw 1 lc rgb "black" title "y = x (Tolerance Bound)"

unset multiplot
