CXXFLAGS += -Wall -Werror -Wextra -std=c++20
LDLIBS += -lstdc++ -lm

TIME = time --format "$$nthreads %e %U" --append --output $@

times.svg: times.gnu Out.times Makefile
	gnuplot $<

N = 1e9
out.times : main #Makefile
	>$@
	>log.main
	for nthreads in 1 2 3 4 5 6 7 8; do \
		$(TIME) ./main -nthreads $$nthreads -nterms $(N) >>log.main ;\
	done

clean:
	$(RM) -f [Oo]ut* log* main *.svg fit.log
