CXXFLAGS += -Wall -Werror -Wextra -std=c++20
LDLIBS += -lstdc++ -lm

Out.txt: main
	./$< > $@

clean:
	$(RM) [Oo]ut* *.o main
