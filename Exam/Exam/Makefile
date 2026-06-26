CXX = g++
CXXFLAGS += -std=c++20 -I. -Wall -Wextra -O3
LDLIBS += -lstdc++ -lm

all: out.txt diagnostics.svg

diagnostics.svg: plot.gnu convergence.txt timing.txt
	gnuplot plot.gnu

out.txt convergence.txt timing.txt: main
	./main > out.txt
	cat out.txt

main: main.o matrix.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

matrix.o: matrix.cc matrix.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: main.cc matrix.h jacobi.h lanczos.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) main *.o *.txt *.svg log
