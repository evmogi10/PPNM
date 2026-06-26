# Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .
set terminal svg background rgb "#ffffff"
set output "hydrogen_wave.svg"
set xlabel "r (Bohr radii)"
set ylabel "f(r) (Wavefunction)"
set title "Hydrogen S-Wave Radial Functions"
set key top right
set grid

# Comparación con analítico: f_0(r) = 2*r*exp(-r)
f0(x) = 2 * x * exp(-x)

plot \
    "hydrogen_wave.data" using 1:2 with lines lw 2 title "Calculated Ground State (k=0)", \
    f0(x) with lines dt 2 lw 2 lc rgb "black" title "Analytical Ground State", \
    "hydrogen_wave.data" using 1:3 with lines lw 2 title "First Excited State (k=1)", \
    "hydrogen_wave.data" using 1:4 with lines lw 2 title "Second Excited State (k=2)"
