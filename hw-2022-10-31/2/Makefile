GCC = gcc
FLAGS = -Werror -Wall -g -fsanitize=address,undefined
RM = rm -rf 2&>/dev/null

run: task2
	./task2 input_original_name_here 4dm1n 3.1415926535

task2: main.o str.o
	${GCC} ${FLAGS} main.o str.o -o task2

main.o: main.c str.h
	${GCC} ${FLAGS} -c main.c

str.o: str.c str.h
	${GCC} ${FLAGS} -c str.c

clean:
	${RM} task2 *.o
