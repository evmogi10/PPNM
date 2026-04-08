CXX = g++
CXXFLAGS += -std=c++20 -I. -Wall -Wextra -Werror
LDLIBS += -lstdc++ -lm

all: out.txt decay_fit.svg

decay_fit.svg: fit.gnu data.txt fit_curves.txt
	gnuplot $<

data.txt fit_curves.txt out.txt: main
	./main > out.txt

main: main.o matrix.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

main.o: main.cc matrix.h qr.h lsfit.h
matrix.o: matrix.cc matrix.h

clean:
	$(RM) main *.o *.txt *.svg
