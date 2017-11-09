all: dynopoly

dynopoly: MonopolyLibrary.cpp main.cpp MonopolyLibrary.h
	g++ -o dynopoly MonopolyLibrary.cpp main.cpp

clean:
	rm dynopoly

test:
	./dynopoly
