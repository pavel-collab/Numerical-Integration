# флаги компиляции
C_FLAGS=-Werror -Wall -Wextra -Wnarrowing
# флаги оптимизации
C_OPTIMIZE_FLAGS=-O2 -flto -fomit-frame-pointer

all:
	gcc -o out $(C_FLAGS) main.c 