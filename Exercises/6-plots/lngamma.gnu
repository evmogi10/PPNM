set terminal svg background rgb "#ffffff"
set output "lngamma.svg"
set xlabel "x"
set ylabel "lngamma(x)"
set key left top
set tics in
set grid xtics
set grid ytics
plot \
	"lngamma.data" using 1:3 with lines title "lngamma", \
	"lngamma-tab.data" using 1:2 with points pointtype 1 title "tab"
