all: qsort.c
	gcc qsort.c -o qsort

debug: qsort.c
	gcc -g qsort.c -o debug -O0

test: debug
	...

clean:
	rm *.o qsort qsort.dbg
