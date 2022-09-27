all: main.out
main.out: matrix.o list.o
	gcc -o main.out main.c matrix.o list.o
matrix.o: matrix.c matrix.h
	gcc -c matrix.c
list.o: list.c list.h
	gcc -c list.c

clean:
	rm -f matrix.o list.o main.out Rplots.pdf
