all: manager.o a.out
	true

a.out: manager.hxx manager.o main.cxx
	g++ manager.o main.cxx -o a.out

manager.o: manager.cxx manager.hxx
	g++ -c manager.cxx

clean:
	rm *.o a.out 2>/dev/null || true
