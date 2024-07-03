libreactor.a: Proactor.o
	ar rcs libProactor.a Proactor.o
	ranlib libProactor.a

Proactor.o: Proactor.cpp Proactor.hpp
	g++ -std=c++2a -c Proactor.cpp

clean:
	rm -f libProactor.a Proactor.o

.PHONY: all clean