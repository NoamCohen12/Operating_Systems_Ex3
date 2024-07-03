.PHONY: all run clean

all: q10

run: q10
	./q10

q10: q10.o Kosaraju.o libreactor.a
	g++ -o q10 q10.o Kosaraju.o -L. -lProactor

q10.o: q10.cpp 
	g++ -c q10.cpp

Kosaraju.o: Kosaraju.cpp
	g++ -c Kosaraju.cpp

libreactor.a: Proactor.o
	ar rcs libProactor.a Proactor.o
	ranlib libProactor.a

Proactor.o: Proactor.cpp Proactor.hpp
	g++ -c Proactor.cpp	

clean:
	rm -f q10 q10.o Kosaraju.o libProactor.a Proactor.o