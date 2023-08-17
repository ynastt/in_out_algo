gcc -o main main.c -lm
valgrind --leak-check=full ./main > log.txt