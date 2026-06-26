# Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested
set terminal svg background rgb "white" size 800,800 enhanced font "Arial,12"
set output "diagnostics.svg"

set multiplot layout 2,1 title "Lanczos Tridiagonalization: Hydrogen Atom Discretization" font ",14 bold"

# Subplot 1: Convergence Analysis
set title "Ground State Energy Convergence vs. Subspace Dimension (n)" font ",11 bold"
set xlabel "Krylov Subspace Size (n)"
set ylabel "Absolute Error vs Full EVD"
set logscale y
set grid my xtics ytics
set yrange [1e-8:10] 
plot "convergence.txt" using 1:3 with linespoints pt 7 ps 0.9 lc rgb "#00008B" lw 2 title "Absolute Error"

# Subplot 2: Execution Efficiency Analysis
unset logscale y
set title "Computational Run Time: Scalability Advantage" font ",11 bold"
set xlabel "Krylov Subspace Size (n)"
set ylabel "Time (milliseconds)"
set grid
plot "timing.txt" using 1:($2*1000) with linespoints pt 5 ps 0.9 lc rgb "#DC143C" lw 2 title "Lanczos Extraction Time"

unset multiplot
