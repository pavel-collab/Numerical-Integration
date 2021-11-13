# флаги компиляции
C_FLAGS=-Werror -Wall -Wextra -Wnarrowing
# флаги оптимизации
C_OPTIMIZE_FLAGS=-O2 -flto -fomit-frame-pointer

all: main

main.o: main.c
	gcc -c main.c
lib.o: lib.c
	gcc -c lib.c

main: lib.o main.o
	gcc -o a.out $(C_FLAGS) lib.o main.o -lpthread -ldl

gnuplot: gnuplot_call.c
	gcc -o gnuplot -Wall -Wextra gnuplot_call.c -ldl

simple_int: simple_int.c
	gcc -o simple_int -Wall -Wextra lib.c simple_int.c -ldl

clear:
	rm *.o a.out