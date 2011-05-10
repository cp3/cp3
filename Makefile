trogdor.o: trogdor.c
	gcc -Wall trogdor.c -o trogdor
run:
	./trogdor < in
clean:
	rm -f trogdor
