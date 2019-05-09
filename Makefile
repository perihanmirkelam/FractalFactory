CC = gcc
CFLAGS = -c

all: FractalGenerator

FractalGenerator: main.o fractal.o
	$(CC) main.o fractal.o -o run

main.o: main.c
	$(CC) $(CFLAGS) main.c

fractal.o: fractal.c
	$(CC) $(CFLAGS) fractal.c

clean:
	rm -rf *o run 


