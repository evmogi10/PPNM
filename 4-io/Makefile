CXXFLAGS = -Wall -Werror -Wextra -std=c++20 -O
LDLIBS = -lstdc++ -lm

Out.txt: main
	./main > Out.txt

main: main.o
	$(CXX) main.o -o main

main.o: main.cc
	$(CXX) $(CXXFLAGS) -c main.cc
.PHONY: clean

clean:
	rm -rf main.o Out.txt
