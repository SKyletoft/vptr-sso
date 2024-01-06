CXX ?= g++
CC ?= gcc

OPT_LEVEL ?= -Og -g3

CXXFLAGS := \
	-std=c++23 \
	-Wall -Wextra -Wpedantic \
	\
	-Wno-unused-parameter \
	-Wfloat-equal \
	-Wnon-virtual-dtor \
	-Wcast-align \
	-Wzero-as-null-pointer-constant \
	-Wunused \
	-Woverloaded-virtual \
	-Wmisleading-indentation \
	-Wduplicated-cond \
	-Wduplicated-branches \
	-Wlogical-op \
	-Wtype-limits \
	-Wnull-dereference \
	\
	-Werror=format-security  \
	-Werror=missing-field-initializers \
	-Werror=return-type \
	-Werror=conversion \
	-Werror=sign-conversion \
	-Werror=float-conversion

run: Set
	./Set

Set: Set.o
	$(CXX) $(LDFLAGS) $(OPT_LEVEL) $< -o $@

%.o: %.cpp %.h
	$(CXX) $(CPPFLAGS) $(OPT_LEVEL) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm Main Set *.cpp *.o
