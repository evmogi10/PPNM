CXXFLAGS += -Wall -Werror -Wextra -std=c++20
LDLIBS += -lstdc++ -lm
CSLIBS = -reference:System.Numerics.dll
CODE = $(filter %.cs,$^)
DLLS = $(addprefix -reference:,$(filter %.dll,$^))
MKEXE = mcs -target:exe -out:$@ $(CSLIBS) $(DLLS) $(CODE)
MKLIB = mcs -target:library -out:$@ $(CSLIBS) $(DLLS) $(CODE)

Out.txt: main.exe main
	mono $< > $@
	./$< >> $@
main.exe : main,cs ; $(MKEXE)
