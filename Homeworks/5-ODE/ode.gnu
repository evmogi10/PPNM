# Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .
set terminal svg background rgb "white" size 800,1000
set output "ode_results.svg"
set multiplot layout 3,1 title "ODE Solver Results" font ",16"

# -------------------------------------------------------------
# Task A: Oscillator
# -------------------------------------------------------------
set title "Task A: Harmonic Oscillator (u'' = -u)"
set xlabel "x"
set ylabel "y"
set key top right
plot "oscillator.txt" using 1:2 with lines lw 2 lc rgb "blue" title "Position (u)", \
     "oscillator.txt" using 1:3 with lines dt 2 lw 2 lc rgb "red" title "Velocity (u')"

# -------------------------------------------------------------
# Task B: Planetary Orbit Precession
# -------------------------------------------------------------
set title "Task B: Planetary Orbit (Converted to Cartesian)"
set xlabel "x"
set ylabel "y"
set size ratio -1
set key top right

# The hint says plot using (1/$2)*cos($1):(1/$2)*sin($1)
plot "orbit_circ.txt" using ((1/$2)*cos($1)):((1/$2)*sin($1)) with lines dt 2 lw 2 lc rgb "black" title "Circular", \
     "orbit_ellip.txt" using ((1/$2)*cos($1)):((1/$2)*sin($1)) with lines dt 4 lw 2 lc rgb "green" title "Elliptical", \
     "orbit_precess.txt" using ((1/$2)*cos($1)):((1/$2)*sin($1)) with lines lw 1.5 lc rgb "red" title "Relativistic Precession"

# -------------------------------------------------------------
# Task C: Three-Body Figure 8
# -------------------------------------------------------------
set title "Task C: Three-Body Figure-8 Configuration"
set xlabel "x"
set ylabel "y"
set size ratio -1
set key bottom right
plot "figure8.txt" using 1:2 with lines lw 2 lc rgb "red" title "Body 1", \
     "figure8.txt" using 3:4 with lines lw 2 lc rgb "blue" title "Body 2", \
     "figure8.txt" using 5:6 with lines lw 2 lc rgb "green" title "Body 3"

unset multiplot
