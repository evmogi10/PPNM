# Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .
CXX = g++
CXXFLAGS += -std=c++20 -I. -Wall -Wextra -O3
LDLIBS += -lstdc++ -lm

all: out.txt mc_results.svg

mc_results.svg: plot.gnu mc_scaling.txt
	gnuplot $<

mc_scaling.txt out.txt: main
	./main > out.txt
	cat out.txt

main: main.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

main.o: main.cc montecarlo.h

clean:
	$(RM) main *.o *.txt *.svg
