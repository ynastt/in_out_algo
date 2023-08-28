gcc -fsanitize=address -o gen -g general.c -lm
valgrind --leak-check=full ./gen > log.txt