# Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .
set terminal svg background rgb "#ffffff"
set output "jacobi_scaling.svg"
set xlabel "Matrix size (N)"
set ylabel "Time (seconds)"
set title "Jacobi Algorithm Complexity Scaling"
set grid

# Ajuste a una función cúbica
f(x) = k * x**3
fit f(x) "jacobi_scaling.data" via k

plot "jacobi_scaling.data" with points pt 7 lc rgb "blue" title "Measured Time", \
     f(x) with lines lc rgb "red" lw 2 title sprintf("Fit O(N^3): k=%.2e", k)
