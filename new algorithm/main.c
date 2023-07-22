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



void slice(const char* str, char* result, size_t start, size_t end)
{
    strncpy(result, str + start, end - start);
}

char * my_octal_add(code1, digits) {
    char *code;

    return code;
}

int main()
{
    unsigned long temp = 0;
    clock_t t;
    
    FILE * f = fopen("../tests/test1.txt", "rt");
    char *code;
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
    printf(" size of file: %ld\n", f_size);

    code = malloc(f_size);
    code1 = malloc(f_size);

    while ((c = fgetc(f)) != EOF) {
        code[n++] = (char)c;
        code1[m++] = (char)c;
        // printf("char: %c\n", (char)c);
    }

    code[n] = '\0'; 
    code1[m] = '\0'; 
    int cursor = 0;
    for (int i = 0; i < f_size; i++) {
        cursor = i + 1;
        printf("\ndigit char: %c\n", code1[i]);
        char* todigits;
        todigits = malloc(f_size);
        slice(code1, todigits, 0, cursor);
        printf("%s\n", code1);
        printf("%s\n", todigits);
        unsigned long long digits = (unsigned long long)atoi(todigits); 
        printf("digit long: %lld\n", digits);
        if (cursor == f_size) {
            break;
        }
        digits = digits * 2 * powl(10, (f_size - 1 - cursor));
        printf("digit * 2 step: %lld\n", digits);
        code1 = my_octal_add(code1, digits); //todo

    }
     
    fclose(f);
    printf("whole number: %s\n", code);
    
    FILE * o = fopen("output.txt", "w");
    fputs(code, o);
    fclose(o);    

    t = clock();
    
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("algorithm took %f seconds to execute \n", time_taken);
    free(code);
    return 0;
}