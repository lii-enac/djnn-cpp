CXX = g++
LEX = flex
YACC = /usr/local/opt/bison/bin/bison -d

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
