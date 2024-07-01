.PHONY: all run clean

all: q4

run: q4
	./q4

q4: q4.o Kosaraju.o
	g++ -o q4 q4.o Kosaraju.o

q4.o: q4.cpp 
	g++ -c q4.cpp

Kosaraju.o: Kosaraju.cpp
	g++ -c Kosaraju.cpp

clean:
	rm -f q4 q4.o Kosaraju.o