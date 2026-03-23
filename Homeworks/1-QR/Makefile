CXXFLAGS += -std=c++20 -I. -Wall -Wextra -Werror #-DLONG_DOUBLE
LDLIBS += -lstdc++ -lm

all: timing.svg Out.txt

timing.svg: timing.gnu out.times.data
	gnuplot $<

out.times.data : main-h 
	>$@
	for N in $$(seq 100 20 400); do \
		echo -n "$$N " >> $@ ; \
		(bash -c "TIMEFORMAT='%R'; time ./main-h -size $$N >/dev/null") 2>&1 | tr ',' '.' >> $@ ; \
	done

Out.txt : main-h
	@echo "\nInterface in .h, implementation in matrix.cc\n"
	> $@
	./$< >> $@

main-h.o : matrix.h qr.h
matrix.o : matrix.h qr.h

main-h : main-h.o matrix.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: clean
clean:
	$(RM) main-h *.o *.data *.svg Out.txt out err
test:
	echo $(findstring clang++,$(CXX)) 
	echo $(CXX)
	echo $(CXXFLAGS)

