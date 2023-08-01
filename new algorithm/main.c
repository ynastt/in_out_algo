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

char* mul_x_2(char* code, long n) {
    printf("\nMULX2: code %s\n", code );
    char c;
    char* code1;
    code1 = malloc(n + 1);
    long digit, dec, res;
    long carry = 0;
    code1[0] = '0';
    for (int i = n - 1; i >= 0; i--) {
        c = code[i];
        printf("\tchar: %c\n", c);
        char *pChar = &c;
        digit = (long)atoi(pChar); 
        printf("\tdigit %ld\n", digit);
        digit = digit * 2;
        printf("\tdigitx2 %ld\n", digit);
        dec = digit / 10;
        res = digit%10 + carry;
        if (res > 9) {
            dec = res /8;
            // code[i] = (res%10) + '0';
            code1[i + 1] = (res%10) + '0';
        } else {
            // code[i] = res + '0';
            code1[i + 1] = res + '0';
        }

        // printf("\tdigit code %c\n", code[i]);
        printf("\tdigit code %c\n", code1[i]);
        if (dec > 0) {
            carry = dec;
        } else {
            carry = 0;
        }
        printf("\tcarry %ld, ost: %ld\n", dec, digit%10);
    }
    code1[n+1] = '\0';
    // code[n] = '\0';
    // printf("\tcode %s\n", code);
    printf("\tcode %s\n", code1);
    return code1;
}

char* my_octal_add(char* code1, char* code2, long n) {
    char *code;
    char c1, c2;
    int digit, digit1, digit2, sd, dec, res, carry = 0;
    code = malloc(n);
    printf("\nOCTAL ADD: first: %s\n", code1);
    printf("OCTAL ADD: second: %s\n", code2);

    c1 = code1[0];
    printf("OCTAL ADD: first char from start: %c\n", c1);
    sd = c1-'0';
    printf("\tfirst digit from start: %d\n", sd);  
    code[0] = sd + '0';
    for (int i = n - 1; i > 0; i--) {
        c1 = code1[i];
        printf("OCTAL ADD: first char: %c\n", c1);
        c2 = code2[i - 1];
        printf("OCTAL ADD: second char: %c\n", c2);
        digit1 = c1-'0'; 
        digit2 = c2-'0'; 
        printf("\tfirst digit: %d\n", digit1);
        printf("\tsecond digit: %d\n", digit2);
        digit = digit1 + digit2;
        dec = digit / 8;
        res = digit%8 + carry;
        if (res > 7) {
            dec = res /8;
            code[i] = (res%8) + '0';
        } else {
            code[i] = res + '0';
        }
        printf("\tdigit code %c\n", code[i]);
        if (dec > 0) {
            carry = dec;
        } else {
            carry = 0;
        }
        printf("\tcarry %d, ost: %d\n", dec, digit%8);
        if (dec > 0 && i == 1) {
            code[0] = sd + carry + '0';
        }
    } 
    code[n] = '\0';
    printf("\tcode %s\n", code);
    return code;
}

char* convert_to_binary(char* code, char* map[]) {
    char* res;
    int len = strlen(code);
    res = malloc(len * 3);
    strcpy(res, "");
    for (int i = 0; i < len; i++) {
        printf("%d - %c\n", i, (char)code[i]);
        int c = code[i] - '0';
        printf("oct: %d; bin: %s\n", c, map[c]);
        strcat(res, map[c]);
        printf("cur res: %s\n", res);
    }
    return res;
}

int main() {
    clock_t t;
    t = clock();
    FILE * f = fopen("../tests/test2.txt", "rt");
    char *code;
    char *code1;
    size_t n = 0, m = 1;
    int c;

    if( f == NULL ) {
        perror("Error: ");
        return(-1);
    }

    fseek(f, 0, SEEK_END);
    long f_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    printf("size of file: %ld\n", f_size);

    code = malloc(f_size);
    code1 = malloc(f_size + 1);
    code1[0] = '0';
    while ((c = fgetc(f)) != EOF) {
        code[n++] = (char)c;
        code1[m++] = (char)c;
        // printf("char: %c\n", (char)c);
    }

    code[n] = '\0'; 
    code1[m] = '\0'; 
    int cursor = 0;
    for (int i = 1; i < f_size + 1; i++) {
        cursor = i + 1;
        printf("\ncurrent digit char: %c\n", code1[i]);
        char* todigits;
        todigits = malloc(f_size);
        slice(code1, todigits, 1, cursor);
        todigits[cursor] = '\0';
        printf("%s\n", code1);
        printf("%s\n", todigits);
        if (cursor == f_size + 1) {
            if (code1[0] == '0') {
                code1++;
            }
            break;
        }
        char* tmp;
        tmp = malloc(f_size);
        tmp = mul_x_2(todigits, f_size - 1);
        printf("tmp: %s\n", tmp);
        
        code1 = my_octal_add(code1, tmp, f_size + 1);
        free(todigits);
    }
     
    fclose(f);
    printf("\nRESULT\ndecimal number: %s\n", code);
    printf("octal number: %s\n", code1);
    FILE * o = fopen("output.txt", "w");
    fputs(code1, o);
    fclose(o);    

    char* map[] = { "000", "001", "010", "011", "100", "101", "110", "111"};
    // printf("octal to binary map:\n");
    // for (int i = 0; i < 8; i++) {
    //     printf("%d -> %s\n", i, map[i]);
    // }
    code1 = convert_to_binary(code1, map);
    printf("binary: %s\n", code1);
    
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("algorithm took %f seconds to execute \n", time_taken);
    free(code);
    return 0;
}