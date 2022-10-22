run: main
	./main

main: main.o complex_num.o
	g++ main.o complex_num.o -o main

main.o: main.cxx complex_num.hxx
	g++ -c main.cxx

complex_num.o: complex_num.cxx complex_num.hxx
	g++ -c complex_num.cxx

clean:
	rm -rf *.o main