set terminal svg background rgb "#ffffff"
set output "timing.svg"
set xlabel "Matrix size N"
set ylabel "Time (seconds)"
set key left top
set grid

f(x) = k * x**3
fit f(x) "out.times.data" via k

plot "out.times.data" using 1:2 with points pt 7 title "Measured", \
     f(x) with lines lc rgb "red" lw 2 title "Fit O(N^3)"
