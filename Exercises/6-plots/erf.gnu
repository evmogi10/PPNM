# Hold on Gemini 3
set terminal svg background rgb "#ffffff"
set output "erf.svg"
set xlabel "x"
set ylabel "y"
set key right bottom
set tics in
set grid xtics
set grid ytics
set xrange [-3:3]
plot \
	"erf.data" using 1:2 with lines title "erf", \
	"erf-tab.data" using 1:2 with points pointtype 1 title "tab" \
