#CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -O
#LDLIBS = -lstdc++ -lm
#Out.txt : main
#	./main > Out.txt

#main : main.o hello.o
#	$(CXX) main.o hello.o -o main $(LDLIBS)

#main.o : hello.h main.cc
#	$(CXX) $(CXXFLAGS) -c main.cc -o main.o

#hello.o: hello.h hello.cc
#	$(CXX) $(CXXFLAGS) -c hello.cc -o hello.o

#.PHONEY:clean
#clean:
#	$(RM) main main.o Out.txt

#test: 
#	@echo CXX
#	@echo $(CXX)
#	@echo $(RM)

Out.txt: hello
	./hello > Out.txt
hello: hello.cc
	c++ -o hello hello.cc
clean:
	rm -f Out.txt hello
