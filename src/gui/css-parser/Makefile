ifndef os
os := $(shell uname -s)

ifeq ($(findstring MINGW,$(os)),MINGW)
os := MinGW
endif
endif

# cross-compile support
ifndef cross_prefix
ifeq ($(os),Darwin)
cross_prefix := llvm-g
endif
#ifeq ($(os),Linux)
cross_prefix := g
#endif
#cross_prefix := arm-none-eabi-
#cross_prefix := em
#cross_prefix := i686-w64-mingw32-
endif

CC := $(cross_prefix)cc
CXX := $(cross_prefix)++

ifeq ($(os),Linux)
CXXFLAGS +=
YACC = bison -d
LD_LIBRARY_PATH=LD_LIBRARY_PATH
debugger := gdb
endif

ifeq ($(os),Darwin)
YACC = /usr/local/opt/bison/bin/bison -d
LEX = /usr/local/opt/flex/bin/flex
LD_LIBRARY_PATH=DYLD_LIBRARY_PATH
# https://stackoverflow.com/a/33589760
debugger := PATH=/usr/bin /Applications/Xcode.app/Contents/Developer/usr/bin/lldb
CXXFLAGS += -I/usr/local/opt/flex/include
LDFLAGS += -L/usr/local/opt/flex/lib
endif

ifeq ($(os),MinGW)
YACC = bison -d
LD_LIBRARY_PATH=PATH
debugger := gdb
endif

LEX = flex

CXXFLAGS = -W -Wall -Wextra -ansi -g -std=c++14 -I../../
LDFLAGS = -L../../../build -ldjnn-core -ldjnn-base -ldjnn-display -ldjnn-gui

HEADERS = driver.h parser.h scanner.h \
    y.tab.h FlexLexer.h location.hh position.hh stack.hh

all: csstest

# Generate scanner and parser

parser.cpp: parser.y
	$(YACC) -o parser.cpp --defines=parser.h parser.y

scanner.cpp: scanner.l
	$(LEX) -o scanner.cpp scanner.l

# Implicit rule to compile c++ files

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Link executable

csstest: main.o parser.o scanner.o driver.o
	$(CXX) $(LDFLAGS) -o $@ main.o parser.o scanner.o driver.o

clean:
	rm -f csstest *.o parser.cpp parser.h scanner.cpp
