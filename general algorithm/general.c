#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>


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

unsigned int my_strlen(const char *s)
{
    unsigned int count = 0;
    while(*s!='\0') {
        if (isdigit(*s)) count++;
        s++;
    }
    return count;
}

char* find_remainder(char* code, long n) {
    bool new_step = false;
    char* result;
    result = malloc(n + 2);
    int j, d;
    char* code1;
    char* first_two_digits;
    first_two_digits = malloc(2);
    long digits, rem, quot, len = strlen(code);
    int first_digit = code[0] - '0', flag = 0;
    if (first_digit < 8) {
        slice(code, first_two_digits, 0, 2);
        first_two_digits[2] = '\0';
        printf("\nfirst two digits: %s\n", first_two_digits);
        digits = atol(first_two_digits);
        printf("first 2 digits: %ld\n", digits);
        quot = digits / 8;
        rem = digits - quot * 8;
        printf("quot and rem: %ld, %ld\n", quot, rem);
        flag = 1;
    } else {
        printf("first one digit: %d\n", first_digit);
        quot = first_digit / 8; // 1;
        rem = first_digit - quot * 8; //0;
        printf("quot and rem: %ld, %ld\n", quot, rem);
    }
    for (j = 0; j < n; j++) {
        if (new_step) {
            new_step = false;
            first_digit = code[0] - '0',
            flag = 0;
            if (first_digit < 8) {
                slice(code, first_two_digits, 0, 2);
                first_two_digits[2] = '\0';
                printf("\nfirst 2 digits: %s\n", first_two_digits);
                digits = atol(first_two_digits);
                quot = digits / 8;
                rem = digits - quot * 8;
                printf("quot and rem: %ld, %ld\n", quot, rem);
                flag = 1;
                free(code1);
            } else {
                printf("first 1 digit: %d\n", first_digit);
                quot = first_digit / 8; // 1;
                rem = first_digit - quot * 8; //0;
                printf("quot and rem: %ld, %ld\n", quot, rem);
                flag = 0;
            }
        }
        int k = 0;
        code1 = malloc(len - 1);
        code1[k++] = (int)quot + '0';
        for (int i = 1; i < len; i++) {
            printf("i: %d\n", i);
            if (flag == 1) {
                i++;
                if (i == len) {
                    printf("here\n");
                    break;
                }
            } 
            d = code[i] - '0';
            printf("next digit: %d\n", d);
            flag = 0;
            if (rem != 0) {
                digits = rem * 10 + d;
                printf(" next 2 digits: %ld\n", digits);
                quot = digits / 8;
                rem = digits - quot * 8;
                printf("quot and rem: %ld, %ld\n", quot, rem);
                // flag = 1;
            } else {
                if (d < 8) {
                    code1[k++] = '0';
                    slice(code, first_two_digits, i, i + 2);
                    first_two_digits[i+2] = '\0';
                    digits = atol(first_two_digits);
                    printf("next 2 digits: %ld\n", digits);
                    quot = digits / 8;
                    rem = digits - quot * 8;
                    printf("quot and rem: %ld, %ld\n", quot, rem);
                    i++;
                    // flag = 1;
                } else {
                    printf("next 1 digit: %d\n", d);
                    quot = d / 8; // 1;
                    rem = d - quot * 8; //0;
                    printf("quot and rem: %ld, %ld\n", quot, rem); 
                    // flag = 0; 
                }  
            }
            code1[k++] = (int)quot + '0';
        }
        result[j] =  (int)rem + '0';
        new_step = true;
        printf("j remainder: %c\n", result[j]);
        printf("current res: %s\n\n", result);
        strcpy(code, code1);
        printf("current number: %s\n", code1);
        printf("old len: %ld\n", len);
        len = my_strlen(code1);
        printf("len: %ld\n\n", len);      
    }
    free(first_two_digits);
    if (len == 1) {
        d = code[0] - '0';
        while (d != 0) {
            printf("last digit: %d\n", d);
            if (d < 8) {
                result[j++] = d +'0';   
            }
            if (d == 8) {
                quot = d / 8;
                rem = d % 8;
                printf("quot and rem: %ld, %ld\n", quot, rem);
                result[j++] = (int)rem + '0';
            }
            d = d/8;
            printf("current res: %s\n\n", result);
            }
    }  
    result[j++] =  '\0';
    printf("res: %s\n\n", result);
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
    long s = strlen(res);
    char* result;
    result = malloc(s);
    result[s] = '\0';
    for (int i = 0; i < s; i++) {
        result[i] = res[s - 1 - i];
    }
    fclose(f);
    printf("\nRESULT\ndecimal number: %s\n", code);
    printf("octal number: %s\n", result);
    FILE * o = fopen("output.txt", "w");
    fputs(result, o);
    fclose(o);    

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("algorithm took %f seconds to execute \n", time_taken);
    free(code);
    free(code1);
    free(res);
    free(result);
    return 0;
}