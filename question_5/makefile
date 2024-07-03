
all: libreactor.a

libreactor.a: q5.o
	ar rcs libreactor.a q5.o
	ranlib libreactor.a

q5.o: q5.cpp q5.hpp
	g++ -std=c++2a -c q5.cpp

clean:
	rm -f libreactor.a q5.o

.PHONY: all clean