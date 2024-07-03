.PHONY: all run clean

all: q9

run: q9
	./q9

q9: q9.o Kosaraju.o libreactor.a
	g++ -o q9 q9.o Kosaraju.o -L. -lProactor

q9.o: q9.cpp 
	g++ -c q9.cpp

Kosaraju.o: Kosaraju.cpp
	g++ -c Kosaraju.cpp

libreactor.a: Proactor.o
	ar rcs libProactor.a Proactor.o
	ranlib libProactor.a

Proactor.o: Proactor.cpp Proactor.hpp
	g++ -c Proactor.cpp	

clean:
	rm -f q9 q9.o Kosaraju.o libProactor.a Proactor.o