.PHONY: all run clean

all: q6

run: q6
	./q6

q6: q6.o Kosaraju.o Reactor.o
	g++ -o q6 -g q6.o Kosaraju.o Reactor.o

q6.o: q6.cpp 
	g++ -c -g q6.cpp

Kosaraju.o: Kosaraju.cpp
	g++ -c -g Kosaraju.cpp

Reactor.o: Reactor.cpp Reactor.hpp
	g++ -c -g Reactor.cpp	

clean:
	rm -f q6 q6.o Kosaraju.o Reactor.o 