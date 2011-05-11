trogdor.o: trogdor.c
	gcc -Wall trogdor.c -o trogdor.o
run:
	./trogdor.o < in
clean:
	rm -f trogdor.o
