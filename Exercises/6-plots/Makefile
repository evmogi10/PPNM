CXXFLAGS += -Wall -Werror -Wextra -std=c++20
LDLIBS += -lstdc++ -lm

all: erf.svg gamma.svg lngamma.svg

erf.svg: erf.gnu erf.data erf-tab.data
	gnuplot $<

gamma.svg: gamma.gnu gamma.data gamma-tab.data
	gnuplot $<

lngamma.svg: lngamma.gnu lngamma.data lngamma-tab.data
	gnuplot $<

main: main.cc
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDLIBS)

erf.data: main Makefile
	./main > $@

gamma.data: main Makefile
	./main -xmin 0 -xmax 3 > $@

lngamma.data: main Makefile
	./main -xmin 0 -xmax 3 -log > $@

clean:
	$(RM) main *.data *.svg log.
