# Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .
CXX = g++
CXXFLAGS += -std=c++20 -I. -Wall -Wextra -O3
LDLIBS += -lstdc++ -lm

all: out.txt integration_results.svg

integration_results.svg: plot.gnu erf_plot.txt erf_conv.txt
	gnuplot $<

erf_plot.txt erf_conv.txt out.txt: main
	./main > out.txt
	cat out.txt

main: main.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

main.o: main.cc integrate.h

clean:
	$(RM) main *.o *.txt *.svg
