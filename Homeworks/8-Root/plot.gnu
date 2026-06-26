# Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .
set terminal svg background rgb "white" size 800,600
set output "plot_results.svg"

set title "Hydrogen S-Wave Radial Wavefunction (Shooting Method)" font ",16"
set xlabel "r (Bohr radii)"
set ylabel "f(r)"
set grid
set key top right

plot "hydrogen_wave.txt" using 1:2 with points pt 7 ps 0.8 lc rgb "red" title "Numerical Result", \
     "hydrogen_wave.txt" using 1:3 with lines lw 2 lc rgb "black" title "Exact f(r) = r * e^{-r}"
