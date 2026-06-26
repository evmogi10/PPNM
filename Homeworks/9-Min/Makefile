# Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested.*/
CXX = g++
CXXFLAGS += -std=c++20 -I. -Wall -Wextra -O3
LDLIBS += -lstdc++ -lm

all: out.txt plot_results.svg

plot_results.svg: plot.gnu out.txt higgs_fit.txt higgs.data.txt
	gnuplot plot.gnu

out.txt higgs_fit.txt: main higgs.data.txt
	./main < higgs.data.txt > out.txt
	cat out.txt

main: main.o matrix.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

matrix.o: matrix.cc matrix.h qr.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: main.cc minimize.h matrix.h qr.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) main *.o *.txt *.svg log
