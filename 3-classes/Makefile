#CXX = g++ -std=c++23
#CXX = clang++ -std=c++23
CXXFLAGS += -Wall -Wextra -Werror -std=c++20 -O
LDLIBS += -lstdc++ -lm

all: Out.txt
	cat $<

Out.txt : main
#	./main > Out.txt
	./$< > $@

main: main.o hello.o vec.o epsilon.o
main.o: hello.h vec.h epsilon.h
epsilon.o: epsilon.h
vec.o: vec.h


clean:
	$(RM) *.o main [Oo]out*
