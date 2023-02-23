all: init print set destroy
	true

init: init.cxx
	g++ init.cxx -o init

print: print.cxx
	g++ print.cxx -o print

set: set.cxx
	g++ set.cxx -o set

destroy: destroy.cxx
	g++ destroy.cxx -o destroy

clean:
	rm init print set destroy 2>/dev/null || true
