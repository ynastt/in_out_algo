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
#define PAIR(q)    ((q)->pair)

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

char* my_reverse(char* code) {
    long s = strlen(code);
    char* result = malloc(s);
    result[s] = '\0';
    for (int i = 0; i < s; i++) {
        result[i] = code[s - 1 - i];
    }
    return result;
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
void print_Link(LINK* l){
    // printf("\nis link null:%u\n", l == NULL);
    if (l != NULL) {
        printf("%ld\n", PAIR(l).n);
        print_Link(PREV(l));
    }
}

void print_Link_64(LINK* l) {
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
        print_Link_64(l->prec->prec);
    }
}

// для перевода макроцифр в с.с 2^32 в 2-ю с.с 
char* convert_to_binary(LINK* l) {
    char* res;
    char* b;
    long k = 0;
    res = malloc(1 * sizeof(char));
    strcpy(res, "");
    while (l != NULL ) {
        res = (char*)realloc(res, k + 32);
        unsigned long digit = l->pair.n;
        printf("digit is: %ld\n", digit);
        if (digit == 0) {
            strcat(res, "0");
        } else {
            b = (char*)malloc(32 * sizeof(char));
            for (int i = 0; digit > 0; i++) {    
                b[i]= (digit % 2) + '0';    
                digit = digit / 2;    
            }
            printf("b: %s\n", b);
            long s = strlen(b);
            char* binary = (char*)malloc(s * sizeof(char));
            binary[s] = '\0';
            for (int i = 0; i < s; i++) {
                binary[i] = b[s - 1 - i];
            } 
            free(b);
            printf("current binary is: %s\n", binary);
            long long len = strlen(binary);
            printf("len: %lld\n", len);
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
            printf("binary is: %s\n", binary);
            strcat(res, binary);
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

int main() {
    clock_t t;
    t = clock();
    FILE * f = fopen("../tests/test2.txt", "rt");
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
    printf("==== IN ALGORITHM ====\n");
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
    char* result = my_reverse(res);
    fclose(f);
    printf("\nRESULT\ndecimal number: %s\n", code);
    printf("octal number: %s\n", result);
    FILE * o = fopen("output.txt", "w");
    fputs(result, o);
    fputs("\n", o);
    char* map[] = { "000", "001", "010", "011", "100", "101", "110", "111"};  
    code1 = convert_from_octal_to_binary(result, map);
    printf("binary: %s\n", code1);
    fputs(code1, o);
    fputs("\n", o);
    printf("2^32:\n");
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
    print_Link(start_number);

    printf("\n2^64:\n");
    print_Link_64(start_number);
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("\nin algorithm took %f seconds to execute \n", time_taken);

    printf("\n==== OUT ALGORITHM ====\n");
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
    code1 = convert_from_binary_to_octal(code1, map);
    printf("octal: %s\n", code1);
    size_t size = strlen(code1);
    printf("size: %ld\n", size);
    int deg = 0;
    unsigned long cur;
    char* dec = (char*) malloc((size + 1) * sizeof(char));
    for (int i = size - 1; i >= 0; i--) {
        cur = (code1[i] - '0') * pow(8, deg);
        printf("\tcur: %ld, cur_len: %d, deg = %d\n", cur, cur_len, deg);
        printf("here\n");
        // char* num = (char*)malloc( cur_len* sizeof(char));
        // printf("here\n");
        // sprintf(num, "%ld", cur);
        // printf("num: %s", num);
        // ++deg;
        // // dec = my_add(dec, num);
        // // printf("\tcurrent res: %s\n", dec);
        // free(num);
    }
    printf("decimal: %s\n", dec);
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("\nout algorithm took %f seconds to execute \n", time_taken);
    fclose(o);
    free(code);
    free(code1);
    free(res);
    free(result);
    free(dec);
    return 0;
}