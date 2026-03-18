set terminal svg background rgb "#ffffff"
set output "times.svg"
set xlabel "number of threads";\
set ylabel "running time";\
a=1;\
b=10;\
f(n)=a+b/n;\
fit f(x) "Out.times" using 1:2 via a,b;\
plot \
	"Out.times" using 1:2 with linespoints title "time", \
		f(x) with lines title "fit 1/n"\
