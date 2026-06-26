# Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .
set terminal svg background rgb "white" size 800,600
set output "plot_results.svg"

set title "Higgs Boson Discovery (Breit-Wigner Fit)" font ",16"
set xlabel "Energy E [GeV/c^2]"
set ylabel "Signal {/Symbol s}(E)"
set grid
set key top right

plot "higgs.data.txt" using 1:2:3 with yerrorbars pt 7 ps 0.8 lc rgb "black" title "Experimental Data", \
     "higgs_fit.txt" using 1:2 with lines lw 2 lc rgb "red" title "Breit-Wigner Fit"
