.PHONY: clean

dino: dino.o
	cc dino.o -o dino

dino.o: dino.c
	cc -c dino.c -o dino.o -lncurses

clean:
	rm -f *.exe