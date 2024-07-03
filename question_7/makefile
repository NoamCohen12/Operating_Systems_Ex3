.PHONY: all run clean

all: q7

run: q7
	./q7

q7: q7.o Kosaraju.o
	g++ -o q7 q7.o Kosaraju.o

q7.o: q7.cpp 
	g++ -c q7.cpp

Kosaraju.o: Kosaraju.cpp
	g++ -c Kosaraju.cpp

clean:
	rm -f q7 q7.o Kosaraju.o