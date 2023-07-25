#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>


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

void slice(const char* str, char* result, size_t start, size_t end) {
    strncpy(result, str + start, end - start);
}

char* find_remainder(char* code, long n) {
    bool new_step = false;
    char* result;
    result = malloc(n);
    int j = 0, d;
    char* code1;
    char* first_two_digits;
    first_two_digits = malloc(2);
    long digits, rem, quot;

    slice(code, first_two_digits, 0, 2);
    first_two_digits[2] = '\0';
    printf("\nfirst two digits: %s\n", first_two_digits);
    digits = atol(first_two_digits);
    printf("first 2 digits: %ld\n", digits);
    quot = digits / 8;
    rem = digits - quot * 8;
    printf("quot and rem: %ld, %ld\n", quot, rem);

    while (j != n) {
        if (new_step) {
            new_step = false;
            slice(code, first_two_digits, 0, 2);
            first_two_digits[2] = '\0';
            printf("\nfirst two digits: %s\n", first_two_digits);
            digits = atol(first_two_digits);
            printf("first 2 digits: %ld\n", digits);
            quot = digits / 8;
            rem = digits - quot * 8;
            printf("quot and rem: %ld, %ld\n", quot, rem);
            free(code1);
        }
        int k = 0;
        code1 = malloc(n - j - 1);
        code1[k++] = (int)quot + '0';
        for (int i = 2; i < n - j; i++) {
            d = code[i] - '0';
            printf("next digit: %c\n", d + '0');
            digits = rem * 10 + d;
            printf("next 2 digits: %ld\n", digits);
            quot = digits / 8;
            rem = digits - quot * 8;
            printf("quot and rem: %ld, %ld\n", quot, rem);
            code1[k++] = (int)quot + '0';
        }
        result[j] =  (int)rem + '0';
        printf("j remainder: %c\n", result[j]);
        printf("current res: %s\n\n", result);
        strcpy(code, code1);
        printf("current number: %s\n\n", code);
        j = j + 1;
        new_step = true;
    }
    free(first_two_digits);
    result[n] =  '\0';
    return result;
}

int main() {
    clock_t t;
    t = clock();
    FILE * f = fopen("../tests/test1.txt", "rt");
    char *code;
    char *res;
    char *code1;
    size_t n = 0, m = 0, k = 0;
    int c;

    if( f == NULL ) {
        perror("Error: ");
        return(-1);
    }

    fseek(f, 0, SEEK_END);
    long f_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    printf("size of file: %ld\n", f_size);

    // unsigned long long decimalnum = 0, remainder, quotient, octalnum=0;
    // size_t i = 1, j;
    // unsigned long long *octalNumber;
    // octalNumber = malloc(f_size);
    res = malloc(f_size);
    code = malloc(f_size);
    code1 = malloc(f_size);
    while ((c = fgetc(f)) != EOF) {
        code[n++] = (char)c;
        code1[m++] = (char)c;
        printf("char: %c\n", (char)c);
    }
    code[n] = '\0'; 
    code1[m] = '\0'; 

    res = find_remainder(code1, f_size);
    fclose(f);
    // printf("\nRESULT\ndecimal number: %lld\n", decimalnum);
    // printf("octal number: %lld\n", octalnum);
    printf("\nRESULT\ndecimal number: %s\n", code);
    printf("octal number: %s\n", res);
    FILE * o = fopen("output.txt", "w");
    fputs(res, o);
    fclose(o);    

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("algorithm took %f seconds to execute \n", time_taken);
    free(code);
    free(res);
    return 0;
}