# Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .
CXX = g++
CXXFLAGS += -std=c++20 -I. -Wall -Wextra -Werror -O3
LDLIBS += -lstdc++ -lm

all: out.txt ode_results.svg

ode_results.svg: ode.gnu oscillator.txt orbit_circ.txt orbit_ellip.txt orbit_precess.txt figure8.txt
	gnuplot $<

oscillator.txt orbit_circ.txt orbit_ellip.txt orbit_precess.txt figure8.txt out.txt: main
	./main > out.txt
	cat out.txt

main: main.o matrix.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

main.o: main.cc matrix.h ode.h
matrix.o: matrix.cc matrix.h

clean:
	$(RM) main *.o *.txt *.svg
