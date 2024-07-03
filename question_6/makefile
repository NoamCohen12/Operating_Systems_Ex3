.PHONY: all run clean

all: q6

run: q6
	./q6

q6: q6.o Kosaraju.o libreactor.a
	g++ -o q6 -g q6.o Kosaraju.o -L. -lreactor

q6.o: q6.cpp 
	g++ -c -g q6.cpp

Kosaraju.o: Kosaraju.cpp
	g++ -c -g Kosaraju.cpp

Reactor.o: Reactor.cpp Reactor.hpp
	g++ -c -g Reactor.cpp	

libreactor.a: Reactor.o
	ar rcs libreactor.a Reactor.o
	ranlib libreactor.a

clean:
	rm -f q6 *.o *.a