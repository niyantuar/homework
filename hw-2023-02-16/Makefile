all: qsort.c
	gcc qsort.c -o qsort

debug: qsort.c
	gcc -g qsort.c -o debug -O0 --coverage

test: debug
	./debug < 1.txt; ./debug < 2.txt; ./debug < 3.txt; ./debug < 4.txt; \
	lcov -c -d . --rc lcov_branch_coverage=1 -o coverage.info; \
	genhtml --branch-coverage -o html coverage.info

clean:
	rm -r *.o qsort debug coverage.info debug-qsort.gcda debug-qsort.gcno 2>/dev/null html || true
