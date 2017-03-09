INCLUDEDIR = ../headers

vpath %.h $(INCLUDEDIR)

CXXFLAGS = -std=c++11 -I $(INCLUDEDIR) -Wall -Wfatal-errors


main: main.cpp chip8.o
	g++ $(CXXFLAGS) main.cpp chip8.o -o main -L/usr/local/lib -lSDL2

chip8.o: chip8.cpp chip8.h
	g++ $(CXXFLAGS) -c chip8.cpp -o chip8.o -L/usr/local/lib -lSDL2

clean:
	rm -f chip8.o main
