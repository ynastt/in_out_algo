#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


/*** Refal structures. ***/
typedef struct link {
    char ptype; /* type of the link */  
    union {
        struct link *b; /* bracket: ptr to the pair */
    char *f; /* function or compound symbol: ptr to label. */
    char c; /* symbol: actual value. */
    /* unsigned long u; /+* unicode symbol */
    unsigned int u;  /* unicode symbol */
    unsigned long n; /*  macro-digit   */
    unsigned short us_1, us_2;
    } pair;
    struct link *prec; /* ptr to preceding link */
    struct link *foll; /* ptr to following link */
} LINK;

#define NEXT(q)    ((q)->foll)
#define PREV(q)    ((q)->prec)
#define TYPE(q)    ((q)->ptype)


int main() {
    clock_t t;
    t = clock();
    FILE * f = fopen("../tests/test1.txt", "rt");
    // char *code;
    char *code1;
    size_t n = 0, m = 0;
    int c;

    if( f == NULL ) {
        perror("Error: ");
        return(-1);
    }

    fseek(f, 0, SEEK_END);
    long f_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    printf("size of file: %ld\n", f_size);

    unsigned long long decimalnum = 0, remainder, quotient, octalnum=0;
    size_t i = 1, j;
    unsigned long long *octalNumber;
    octalNumber = malloc(f_size);


    while (fscanf(f, "%llu", &decimalnum) == 1)
        printf("dec number: %llu\n", decimalnum);

    quotient = decimalnum;
 
    //Storing remainders until number is equal to zero
    while (quotient != 0)
    {
        octalNumber[i++] = quotient % 8;
        quotient = quotient / 8;
    }
 
    //Converting stored remainder values in corresponding octal number
    for (j = i - 1; j > 0; j--)
        octalnum = octalnum*10 + octalNumber[j];
     

    fclose(f);
    printf("\nRESULT\ndecimal number: %lld\n", decimalnum);
    printf("octal number: %lld\n", octalnum);
    FILE * o = fopen("output.txt", "w");
    fprintf(o, " %llu", octalnum);
    fclose(o);    

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("algorithm took %f seconds to execute \n", time_taken);
    // free(code);
    return 0;
}