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

char* convert_from_decimal_to_octal(char* code, long n) {
    bool new_step = false;
    char* result;
    result = (char*)malloc((n + 1) * sizeof(char));
    int j = 0, d;
    char* code1;
    char* first_two_digits;
    first_two_digits = malloc(2);
    long digits, rem, quot, len = n;
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
    // for (j = 0; j < n - 1; j++) {
    while( len != 1) {    
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
        printf("here11\n");
        code1 = malloc(len);
        printf("here22\n");
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
            printf("check quot and rem: %ld, %ld\n", quot, rem);
            flag = 0;
            if (rem != 0) {
                digits = rem * 10 + d;
                printf(" next 2 digits: %ld\n", digits);
                quot = digits / 8;
                rem = digits - quot * 8;
                printf(" quot and rem: %ld, %ld\n", quot, rem);
                // flag = 1;
            } else {
                if (d < 8 && i != len - 1) {
                    code1[k++] = '0';
                    slice(code, first_two_digits, i, i + 2);
                    first_two_digits[i+2] = '\0';
                    digits = atol(first_two_digits);
                    printf(" next 2 digits: %ld\n", digits);
                    quot = digits / 8;
                    rem = digits - quot * 8;
                    printf(" quot and rem: %ld, %ld\n", quot, rem);
                    i++;
                    // flag = 1;
                } else {
                    printf(" next 1 digit: %d\n", d);
                    quot = d / 8; // 1;
                    rem = d - quot * 8; //0;
                    printf(" quot and rem: %ld, %ld\n", quot, rem); 
                    // flag = 0; 
                }  
            }
            code1[k++] = (int)quot + '0';
        }
        result[j++] =  (int)rem + '0';
        new_step = true;
        printf("current res: %s\n\n", result);
        code1[k] = '\0';
        printf("k: %d\n", k);
        memcpy(code, code1, k);
        printf("---current number: %s---\n", code1);
        printf("old len: %ld\n", len);
        len = k;
        printf("len: %ld\n", len);
              
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
        printf("%d - %c\n", i, (char)code[i]);
        int c = code[i] - '0';
        printf("oct: %d; bin: %s\n", c, map[c]);
        strcat(res, map[c]);
        printf("cur res: %s\n", res);
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
        // printf("i: %ld\n", i);
        slice(code, bin, i - 3, i);
        d = octal_digit(map, bin);
        // printf("%s - %d\n", bin, d);
        char digit[2];
        sprintf(digit, "%d", d);
        res[j] = (digit[0] -'0') + '0';
        j++;
        // printf("cur res: %s\n", res);
    }
    res = my_reverse(res);
    res[j] = '\0';
    return res;
}

// функция для сложения столбиком
char* large_addition(char* num1, char* num2) {
    printf("FIRST: %s\nSECOND %s\n", num1, num2);
    char* num_1;
    char* num_2;
    unsigned long len1 = strlen(num1);
    unsigned long len2 = strlen(num2);
    unsigned long i, j, len = 0;
    char* help;
    if (len1 > len2){
        len = len1 - len2;
        help = (char*)malloc((len1 + 1) * sizeof(char));
        help[len1] = '\0';
        for (i = 0; i < len; i++) {
            help[i] = '0';
        }
        for (i = len, j = 0; i < len2 + len; i++, j++) {
            help[i] = num2[j];
        }
        num_2 = (char*)malloc((len1 + 1) * sizeof(char));
        num_2[len1] = '\0';
        for (i = 0; i < len1; i++) {
            num_2[i] = help[i];
        }
        num_1 =(char*)malloc((len1 + 1) * sizeof(char));
        num_1[len1] = '\0';
        for (i = 0; i < len1; i++) {
            num_1[i] = num1[i];
        }
        free(help);
    } else if (len2 > len1) {
        len = len2 - len1;
        help = (char*)malloc((len2 + 1) * sizeof(char));
        help[len2] = '\0';
        for (i = 0; i < len; i++) {
            help[i] = '0';
        }
        for (i = len, j = 0; i < len1 + len; i++, j++) {
            help[i] = num1[j];
        }
        num_1 = (char*)malloc((len2 + 1) * sizeof(char));
        num_1[len2] = '\0';
        for (i = 0; i < len2; i++) {
            num_1[i] = help[i];
        }    
        num_2 =(char*)malloc((len2 + 1) * sizeof(char));
        num_2[len2] = '\0';
        for (i = 0; i < len2; i++) {
            num_2[i] = num2[i];
        }
        free(help);
    } else {
        num_1 = (char*)malloc((len1 + 1) * sizeof(char));
        num_1[len1] = '\0';
        num_2 = (char*)malloc((len2 + 1) * sizeof(char));
        num_2[len2] = '\0';
        for (i = 0; i < len1; i++) {
            num_1[i] = num1[i];
        }
        for (i = 0; i < len2; i++) {
            num_2[i] = num2[i];
        }
    }
    len = strlen(num_1);
    char* res = (char*)malloc((len + 2) * sizeof(char));
    res[len + 1] = '\0';
    for (i = 0; i < len + 1; i++) {
        res[i] = '0';
    }
    for (i = len, j = len - 1; i > 0; i--, j--) {
        int num = (num_1[j] - '0') + (num_2[j] - '0');
        int dop = (res[i] - '0') + num;
        res[i] = ((((res[i]-'0') + num) % 10) + '0');
        res[i-1] = ((((res[i - 1] - '0') + dop) / 10) + '0');
    }
    if (res[0] == '0') {
        len = strlen(res);
        help = (char*)malloc(len * sizeof(char));
        help[len - 1] = '\0';
        for (j = 1; j < len; j++) {
            help[j - 1] = res[j];
        }
        return help;
    }
    return res;
}

char* large_mul_digit(int d, char* num) {
    unsigned long long i, len = strlen(num);
    printf("num: %s\n", num);
    printf("d: %d\n", d);
    printf("len: %lld\n", len);
    long digit, dec, res, carry = 0;
    int dd;
    char* result = (char*)calloc((len + 2), sizeof(char));
    for (i = 0; i < len + 2; i++) {
        result[i] = '0';
    }
    printf("cur res: %s, %ld\n", result, strlen(result));
    for (i = len + 1; i > 1; i--) {
        printf("\ti: %lld\n", i);
        dd = num[i - 2] - '0';
        printf("\tdd: %d\n", dd);
        dd = d * dd;
        printf("\tdd x d: %d\n", dd);
        dec = dd / 10;
        res = dd%10 + carry;
        printf("\tdec %ld, res: %ld\n", dec, res);
        if (res > 9) {
            dec += res / 10;
            result[i] = (res%10) + '0';
        } else {
            result[i] = res + '0';
        }
        printf("\tdec %ld, res: %ld\n", dec, res);
        printf("\tcur res: %s\n", result);
        if (dec > 0) {
            carry = dec;
        } else {
            carry = 0;
        }
        printf("\tcarry %ld, ost: %d\n", dec, dd%10);
    }
    printf("\ni: %lld\n", i);
    printf("\tcarry %ld\n", dec);
    if (i == 1 && carry > 0) {
        result[i] = (result[i] - '0') + carry + '0';
        carry = 0;
    }
    printf("cur res: %s\n", result);
    while (result[0] == '0') {
        result++;
    }
    result[len + 2] = '\0';
    return result;
}

char* my_pow(int base, unsigned long long deg) {
    char* res = (char*)malloc((deg + 1)* sizeof(char));
    unsigned long long i;
    int dec, carry = 0, r;
    int d = 0;
    res[0] = '1';
    if (deg != 0) {
        for (int count = 1; count <= deg; count++) {
            
            printf("\n\tcount is: %d\n", count);
            printf("====current res: %s======\n", res);
            res[deg + 1] = '\0';
            res = large_mul_digit(base, res);
        }
    }
    while (res[0] == '0') {
        res++;
    }
    res[deg + 1] = '\0';
    return res;
}

int main() {
    clock_t t;
    t = clock();
    FILE * f = fopen("../tests/test5.txt", "rt");
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
        // printf("char: %c\n", (char)c);
    }
    code[n] = '\0'; 
    code1[m] = '\0'; 

    res = convert_from_decimal_to_octal(code1, f_size);
    long s = strlen(res);
    char* result = my_reverse(res);
    result[(strlen(result) + 1)] = '\0';
    fclose(f);
    printf("\ndecimal number: %s\n", code);
    printf("octal number: %s\n", result);
    FILE * o = fopen("output.txt", "w");
    fputs("===IN===\ndecimal: ", o);
    fputs(code, o);
    fputs("\noctal: ", o);
    fputs(result, o);
    char* map[] = { "000", "001", "010", "011", "100", "101", "110", "111"};  
    code1 = convert_from_octal_to_binary(result, map);
    printf("binary: %s\n", code1);
    fputs("\nbinary: ", o);
    fputs(code1, o);
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
    fputs("\nbinary: ", o);
    printf("binary: %s\n", code1);
    fputs(code1, o);
    code1 = convert_from_binary_to_octal(code1, map);
    printf("octal: %s\n", code1);
    fputs("\noctal: ", o);
    fputs(code1, o);
    size_t size = strlen(code1);
    printf("size: %ld\n", size);
    int deg = 0, d = 0;

    // //test
    // char* num = (char*)malloc((15 + 1) * sizeof(char));
    // num = large_mul_digit(2, "581474976710656");
    // printf("\tnum: %s\n", num);

    // printf("\n\npow(8, deg)\n");
    // char* rr = my_pow(8, 16);
    // printf("\tpow(8, 16): %s\n", rr);
    // // end test 

    // unsigned long cur;
    char* dec = (char*) malloc((size + 1) * sizeof(char));
    for (int i = size - 1; i >= 0; i--) {
        d = code1[i] - '0';
        printf("\td: %d\n", d);
        printf("\tpow(8, deg): %s\n", my_pow(8, deg));
        // cur = d * pow(8, deg);
        // printf("\tcur: %ld, deg = %d\n", cur, deg);
        // char* num = (char*)malloc((size + 1) * sizeof(char));
        // // sprintf(num, "%ld", cur);
        char* num = large_mul_digit(d, my_pow(8, deg));
        // num[strlen(num) + 1] = '\0';
        printf("\tnum: %s\n", num);
        ++deg;
        dec = large_addition(dec, num); 
        printf("\t\tcurrent res: %s\n", dec);
        // free(num);
    }
    dec[strlen(dec) + 1] = '\0';
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
    fclose(o);
    free(code);
    free(code1);
    free(res);
    free(result);
    free(dec);
    return 0;
}