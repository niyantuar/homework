all: init print set destroy
	true

init: init.cxx consts.o
	g++ init.cxx consts.o -o init

print: print.cxx consts.o
	g++ print.cxx consts.o -o print

set: set.cxx consts.o
	g++ set.cxx consts.o -o set

destroy: destroy.cxx consts.o
	g++ destroy.cxx consts.o -o destroy

consts.o: consts.cxx consts.hxx
	g++ -c consts.cxx

clean:
	rm init print set destroy consts.o 2>/dev/null || true
