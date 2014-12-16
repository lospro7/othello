# Makefile for othello

Othello1.0 : main.o game.o othello.o piece.o
	g++ -o Othello1.0 game.o othello.o main.o piece.o

main.o: main.cc game.h othello.h colors.h piece.h
	g++ -c main.cc

game.o: game.cc game.h
	g++ -c game.cc

othello.o: othello.cc othello.h
	g++ -c othello.cc

piece.o: piece.cc piece.h
	g++ -c piece.cc

clean:
	rm *.o Othello1.0

# End of Makefile


