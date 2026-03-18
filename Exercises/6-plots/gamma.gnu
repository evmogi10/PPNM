set terminal svg background rgb "#ffffff"
set output "gamma.svg"
set xlabel "x"
set ylabel "gamma(x)"
set key left top
set tics in
set grid xtics
set grid ytics
plot \
	"gamma.data" using 1:3 with lines title "gamma", \
	"gamma-tab.data" using 1:2 with points pointtype 1 title "tab"
