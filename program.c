#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void generate_test_number(unsigned long long len){
    char* number = (char*)malloc((len + 1) * sizeof(char));
    FILE* f = fopen("tests/test.txt", "w");
    for (unsigned long long i = 0; i < len; i++) {
        fprintf(f, "%d", rand()%10);
    }
    fprintf(f,"\n");
    fclose(f);
}


// gcc program.c
// ./a.out num1 num2
int main(int argc, char *argv[]) {
    if( argc == 3 ) {
        printf("Number of iterations: %s\n", argv[1]);
        printf("Length of generated test numbers: %s\n", argv[2]);
        generate_test_number(atoi(argv[2]));

    }
    else if( argc > 3 ) {
        printf("Too many arguments.\n");
        printf("Write next command:   ./a.out <Iters> <In>\n");
        printf("where:\n\t<Iters> -  number of iterations.\n\t<In> - length of generating test number;\n");
    } else {
        printf("Write next command:   ./a.out <Iters> <In>\n");
        printf("where:\n\t<Iters> -  number of iterations.\n\t<In> - length of generating test number;\n");
    }
    return 0;
}