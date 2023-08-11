#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

/*** Refal structure. ***/
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
#define PAIR(q)    ((q)->pair)

void slice(const char* str, char* result, size_t start, size_t end)
{
    strncpy(result, str + start, end - start);
}

char* my_reverse(char* code) {
    long s = strlen(code);
    char* result = malloc(s);
    result[s] = '\0';
    for (int i = 0; i < s; i++) {
        result[i] = code[s - 1 - i];
    }
    return result;
}

char* mul_x_2(char* code, long n) {
    printf("\nMULX2: code %s\n", code );
    char c;
    int i;
    char* code1;
    code1 = malloc(n + 1);
    long digit, dec, res;
    long carry = 0;
    code1[0] = '0';
    for (i = n - 1; i >= 0; i--) {
        c = code[i];
        char *pChar = &c;
        digit = (long)atoi(pChar); 
        printf("\tdigit %ld\n", digit);
        digit = digit * 2;
        printf("\tdigitx2 %ld\n", digit);
        dec = digit / 10;
        res = digit%10 + carry;
        if (res > 9) {
            dec = res /8;
            code1[i + 1] = (res%10) + '0';
        } else {
            code1[i + 1] = res + '0';
        }
        printf("\tdigit code %c\n", code1[i]);
        if (dec > 0) {
            carry = dec;
        } else {
            carry = 0;
        }
        printf("\tcarry %ld, ost: %ld\n", dec, digit%10);
    }
    if (i == -1 && carry > 0) {
        printf("PLUS CARRY\n");
        code1[0] = (long)(code1[0] - '0') + carry + '0';
    }
    code1[n+1] = '\0';
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

// из 8-й в 2-ю с.с 
char* convert_from_octal_to_binary(char* code, char* map[]) {
    char* res;
    int len = strlen(code);
    res = malloc(len * 3);
    strcpy(res, "");
    for (int i = 0; i < len; i++) {
        // printf("%d - %c\n", i, (char)code[i]);
        int c = code[i] - '0';
        // printf("oct: %d; bin: %s\n", c, map[c]);
        strcat(res, map[c]);
        // printf("cur res: %s\n", res);
    }
    res[len * 3 + 1] = '\0';
    return res;
}

// аргументом функции является строка длинной <= 32 символа
unsigned long convert_to_2_in_32_system(char* code) {
    unsigned long res = 0;
    unsigned long cur;
    int deg = 0;
    int len = strlen(code);
    // printf("\tlen of binary: %d\n", len);
    for (int i = len - 1; i >= 0; i--) {
        cur = code[i] - '0';
        // printf("\tcur: %ld", cur);
        res += cur * pow(2, deg);
        ++deg;
        // printf("\tcurrent res: %ld", res);
    }
    // printf("\tres: %ld\n", res);
    return res;
}

// Поскольку я заполняю структуры в направлении с конца в начало,
// то при печати в направлении с начала в конец испольщуется PREV, а не NEXT
// но это можно поправить и сделать наоборот, если критично.
void print_Link(LINK* l, FILE* output){
    // printf("\nis link null:%u\n", l == NULL);
    if (output != NULL) {
        if (l != NULL) {
            char* digit = malloc(20);
            sprintf(digit, "%ld", PAIR(l).n);
            fputs(digit, output);
            fputs("\n", output);
            print_Link(PREV(l), output);
        }
    } else {
        if (l != NULL) {
            printf("%ld\n", PAIR(l).n);
            print_Link(PREV(l), NULL);
        }
    }
}

void print_Link_64(LINK* l, FILE* output) {
    if (output != NULL) {
        if (l != NULL && PREV(l) != NULL) {
            unsigned long senior = l->pair.n;
            unsigned long junior = l->prec->pair.n;
            char* macrodigit = malloc(40);
            char* jun = malloc(20);
            sprintf(macrodigit, "%ld", senior);
            sprintf(jun, "%ld", junior);
            strcat(macrodigit, jun);
            fputs(macrodigit, output);
            print_Link_64(l->prec->prec, output);
        }
    } else {
        if (l != NULL && PREV(l) != NULL) {
            unsigned long senior = l->pair.n;
            unsigned long junior = l->prec->pair.n;
            // printf("link: %ld\n", senior);
            // printf("prev: %ld\n", junior);
            char* macrodigit = malloc(40);
            char* jun = malloc(20);
            sprintf(macrodigit, "%ld", senior);
            sprintf(jun, "%ld", junior);
            strcat(macrodigit, jun);
            printf("%s",macrodigit);
            print_Link_64(l->prec->prec, NULL);
        }
    }
}

// для перевода макроцифр в с.с 2^32 в 2-ю с.с 
char* convert_to_binary(LINK* l) {
    char* res;
    char* b;
    int i = 0;
    unsigned long long length = 0;
    res = malloc(1 * sizeof(char));
    strcpy(res, "");
    while (l != NULL ) {
        res = (char*)realloc(res, length + 32 + 1);
        unsigned long digit = l->pair.n;
        printf("digit is: %ld\n", digit);
        if (digit == 0) {
            strcat(res, "0");
            length++;
            res[length + 1] = '\0';
        } else {
            b = (char*)malloc(33 * sizeof(char));
            for (i = 0; digit > 0; i++) {    
                b[i]= (digit % 2) + '0';    
                digit = digit / 2;
                printf("digit: %ld, ost: %c\n", digit, (char)b[i]);    
            }
            b[i] = '\0';
            printf("b: %s\n", b);
            long s = strlen(b);
            char* binary = (char*)malloc((s + 1)* sizeof(char));
            binary[s] = '\0';
            for (int i = 0; i < s; i++) {
                binary[i] = b[s - 1 - i];
            } 
            free(b);
            printf("current binary is: %s\n", binary);
            long long len = strlen(binary);
            // printf("len: %lld\n", len);
            if (l->foll != NULL && l->foll->pair.n != 0) {
                binary = my_reverse(binary);
                while (len % 32 != 0) {
                    binary[len++] = '0';
                    binary[len++] = '\0';
                    len = strlen(binary);
                } 
            } else {
                binary = my_reverse(binary);
            }
            printf("binary is: %s\n", my_reverse(binary));
            strcat(res, binary);
            length += len;
            res[length + 1] = '\0';
        }   
        printf("current res: %s\n", res);
        l = l->foll;
    }
    return res;
}

int octal_digit(char* map[], char* bin) {
    int res = -1;
    for (int i = 0; i < 8; i++) {
        if (strcmp(bin, map[i]) == 0) return i;
    }
    return res;
}

// из 2-й в 8-ю с.с 
char* convert_from_binary_to_octal(char* code, char* map[]) {
    long len = strlen(code);
    char* res = malloc(len + 1);
    long j = 0;
    int d;
    char bin[3];
    for (long i = len; i > 0; i -= 3) {
        printf("i: %ld\n", i);
        slice(code, bin, i - 3, i);
        d = octal_digit(map, bin);
        printf("%s - %d\n", bin, d);
        char digit[2];
        sprintf(digit, "%d", d);
        res[j] = (digit[0] -'0') + '0';
        j++;
        printf("cur res: %s\n", res);
    }
    res = my_reverse(res);
    res[j] = '\0';
    return res;
}

char* my_decimal_sub(char* code1, char* code2, long n) {
    char *code;
    int digit, digit1, digit2, sd, dec, res, carry = 0;
    code = malloc(n);
    // printf("\nDEC SUB: first: %s\n", code1);
    // printf("DEC SUB: second: %s\n", code2);
    sd = code1[0] - '0';
    // printf("\tfirst digit from start: %d\n", sd);  
    code[0] = sd + '0';
    for (int i = n - 1; i > 0; i--) {
        digit1 = code1[i]-'0'; 
        digit2 = code2[i]-'0'; 
        // printf("\tfirst digit: %d\n", digit1);
        // printf("\tsecond digit: %d\n", digit2);
        digit = digit1 - digit2 - carry;
        if (digit < 0) {
            carry = 1;
            code[i] = (digit + 10) + '0';
        } else {
            carry = 0;
            code[i] = digit + '0';
        }
        // printf("\tdigit code %c\n", code[i]);
        if (carry != 0 && i == 1) {
            code[0] = sd - carry + '0';
        }
    } 
    code[n] = '\0';
    // printf("\tcode %s\n", code);
    return code;
}

int main() {
    clock_t t;
    t = clock();
    FILE * f = fopen("../tests/test.txt", "rt");
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
    printf("==== IN ALGORITHM ====\n");
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
        // printf("\ncurrent digit char: %c\n", code1[i]);
        char* todigits;
        todigits = malloc(f_size);
        slice(code1, todigits, 1, cursor);
        todigits[cursor] = '\0';
        // printf("%s\n", code1);
        // printf("%s\n", todigits);
        if (cursor == f_size + 1) {
            if (code1[0] == '0') {
                code1++;
            }
            break;
        }
        char* tmp;
        tmp = malloc(f_size);
        tmp = mul_x_2(todigits, f_size - 1);
        // printf("tmp: %s\n", tmp);
        
        code1 = my_octal_add(code1, tmp, f_size + 1);
        free(todigits);
    }
     
    fclose(f);
    printf("\ndecimal number: %s\n", code);
    printf("octal number: %s\n", code1);
    FILE * o = fopen("output.txt", "w");
    fputs("===IN===\ndecimal: ", o);
    fputs(code, o);
    fputs("\noctal: ", o);
    fputs(code1, o);
    char* map[] = { "000", "001", "010", "011", "100", "101", "110", "111"};
    // printf("octal to binary map:\n");
    // for (int i = 0; i < 8; i++) {
    //     printf("%d -> %s\n", i, map[i]);
    // }
    code1 = convert_from_octal_to_binary(code1, map);
    printf("binary: %s\n", code1);
    fputs("\nbinary: ", o);
    fputs(code1, o);
    fputs("\n", o);
    printf("2^32:\n");
    fputs("\n\n2^32:\n", o);
    LINK* start_number = (LINK*)malloc(sizeof(LINK));
    LINK* end_number;
    LINK* prev_number;
    prev_number = NULL;
    bool flag_chetn = false;
    unsigned long long old_len, len = strlen(code1);
    while (len > 0) {
        // printf("\nlen: %lld\n", len);
        LINK* number;
        number = (LINK*)malloc(sizeof(LINK));
        if (prev_number != NULL) {
            NEXT(prev_number) = number;
            // printf("link nxt of prev: %ld\n", prev_number->foll->pair.n);
        }
        TYPE(number) = 'd';
        PREV(number) = prev_number;
        char* str = malloc(32 * sizeof(char));
        old_len = len;
        if (len >= 32) {
            slice(code1, str, len-32, len);
            len -= 32;
        } else {
            slice(code1, str, 0, len);
            len -= len;
        }
        
        // printf("str: %s\n", str);
        PAIR(number).n = convert_to_2_in_32_system(str);
        if (old_len == strlen(code1)) {
            end_number = number;
        }
        // printf("link type: %c\n", number->ptype);
        // printf("link n:%ld\n", number->pair.n);
        // printf("link prec is null?:%u\n", number->prec == NULL);
        prev_number = number;
        // printf("\nis prev_link null_1:%u\n", prev_number == NULL);
        
        flag_chetn = !flag_chetn;
    }
    
    if (len == 0) {
        if (flag_chetn) {
            LINK* number = (LINK*)malloc(sizeof(LINK));
            if (prev_number != NULL) {
                NEXT(prev_number) = number;
            }
            TYPE(number) = 'd';
            PREV(number) = prev_number;
            PAIR(number).n = 0;
            start_number = number;
        } else {
            start_number = prev_number;
        }
    }
    print_Link(start_number, NULL);
    print_Link(start_number, o);
    printf("\n2^64:\n");
    fputs("\n2^64:\n", o);
    print_Link_64(start_number, NULL);
    print_Link_64(start_number, o);

    // printf("\nend number: %ld\n", end_number->pair.n);
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("\nin algorithm took %f seconds to execute \n", time_taken);
    fputs("\nin algorithm took ", o);
    char time_arr[100];
    sprintf(time_arr, "%2.7f", time_taken);
    fputs(time_arr, o);
    fputs(" seconds to execute \n", o);
    printf("\n==== OUT ALGORITHM ====\n");
    fputs("\n===OUT===\n2^64:\n", o);
    print_Link_64(start_number, o);
    fputs("\n\n2^32:\n", o);
    print_Link(start_number, o);
    t = clock();
    code1 = convert_to_binary(end_number);
    long long l = strlen(code1);
    while (l % 3 != 0) {
       code1[l++] = '0';
       code1[l++] = '\0';
       l = strlen(code1);
    }
    code1 = my_reverse(code1);
    printf("binary: %s\n", code1);
    fputs("\nbinary: ", o);
    fputs(code1, o);
    code1 = convert_from_binary_to_octal(code1, map);
    printf("octal: %s\n", code1);
    fputs("\noctal: ", o);
    fputs(code1, o);
    size_t size = strlen(code1);
    // printf("size: %ld\n", size);
    for (int i = 0; i < size; i++) {
        cursor = i + 1;
        if (cursor == size) {
            break;
        }
        // printf("\ncurrent digit char: %c\n", code1[i]);
        char* todigits;
        todigits = malloc(size);
        slice(code1, todigits, 0, cursor);
        todigits[cursor] = '\0';
        // printf("cur: %s\n", todigits);
        char* tmp;
        tmp = malloc(size);
        tmp = mul_x_2(todigits, size - 1);
        // printf("tmp: %s\n", tmp);
        code1 = my_decimal_sub(code1, tmp, size);
        code1[size + 1] = '\0';
        free(todigits);
    }
    char* dec = (char*)malloc((size + 1) * sizeof(char));
    memcpy(dec, code1, size + 1);
    while (dec[0] == '0') {
        dec++;
    }
    printf("decimal: %s\n", dec);
    fputs("\ndecimal: ", o);
    fputs(dec, o);
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("\nout algorithm took %f seconds to execute \n", time_taken);
    fputs("\nout algorithm took ", o);
    sprintf(time_arr, "%2.7f", time_taken);
    fputs(time_arr, o);
    fputs(" seconds to execute \n", o);
    free(code);
    free(code1);
    fclose(o);
    return 0;
}
