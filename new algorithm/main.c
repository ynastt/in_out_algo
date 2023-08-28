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

// Вспомогательная функция получения подстроки
// str - исходная строка, result - возвращаемая подстрока,
// start - индекс начала подстроки в строке, 
// end - индекс конца подстроки в строке.
void slice(const char* str, char* result, size_t start, size_t end) {
    strncpy(result, str + start, end - start);
    result[end - start] = '\0';
}

// Вспомогательная функция, возвращает перевернутую строку.
char* my_reverse(char* code) {
    long s = strlen(code);
    char* result = calloc(s + 1, sizeof(char));
    for (int i = 0; i < s; i++) {
        result[i] = code[s - 1 - i];
    }
    result[s] = '\0';
    return result;
}

char* mul_x_2(char* code, long n) {
    printf("\nMULX2: code %s\n", code );
    char c;
    int i;
    char* code1;
    code1 = calloc(n + 1, sizeof(char));
    long digit, dec, res;
    long carry = 0;
    code1[0] = '0';
    for (i = n - 1; i >= 0; i--) {
        c = code[i];
        char *pChar = &c;
        digit = (long)atoi(pChar); 
        printf("\tmul_x_2_>: digit %ld\n", digit);
        digit = digit * 2;
        printf("\tmul_x_2_>: digitx2 %ld\n", digit);
        dec = digit / 10;
        res = digit%10 + carry;
        if (res > 9) {
            dec = res / 10;
            code1[i + 1] = (res%10) + '0';
        } else {
            code1[i + 1] = res + '0';
        }
        printf("\tmul_x_2_>: digit code %c\n", code1[i]);
        if (dec > 0) {
            carry = dec;
        } else {
            carry = 0;
        }
        printf("\tmul_x_2_>: carry %ld, ost: %ld\n", dec, digit%10);
    }
    if (i == -1 && carry > 0) {
        printf("mul_x_2_>: PLUS CARRY\n");
        code1[0] = (long)(code1[0] - '0') + carry + '0';
    }
    code1[n+1] = '\0';
    printf("\tmul_x_2_>: result: %s\n", code1);
    return code1;
}

char* mul_x_2_octal(char* code, long n) {
    printf("\nMULX2: code %s\n", code );
    char c;
    int i;
    char* code1;
    code1 = calloc(n + 1, sizeof(char));
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
        dec = digit / 8;
        res = digit%8 + carry;
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
    code = calloc(n+1, sizeof(char));
    printf("\nOCTAL ADD: first: %s\n", code1);
    printf("OCTAL ADD: second: %s\n", code2);
    unsigned long long i, len1 = strlen(code1), len2 = strlen(code2);
    c1 = code1[0];
    printf("OCTAL ADD: first char from start: %c\n", c1);
    sd = c1-'0';
    printf("\tfirst digit from start: %d\n", sd);  
    printf("len1: %lld\n", len1);
    printf("len2: %lld\n", len2);
    code[0] = sd + '0';
    if (len1 == len2) {
        for (i = len1 - 1; i > 0; i--) {
            printf("i: %lld\n", i);
            c1 = code1[i];
            printf("OCTAL ADD: first char: %c\n", c1);
            c2 = code2[i];
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
            printf("\tdigit code %c\n", (char)code[i]);
            if (dec > 0) {
                carry = dec;
            } else {
                carry = 0;
            }
            printf("\tcarry %d\n", dec);
            if (dec > 0 && i == 1) {
                code[0] = sd + carry + '0';
                // res = (sd + carry)%8;
                // code[0] = res + '0';
                // carry = (sd + carry)/8;
            }
        } 
        if (sd != 0) {
            printf("i: %lld\n", i);
            printf("OCTAL ADD: first char: %c\n", sd + '0');
            printf("OCTAL ADD: second char: %c\n", (char)code2[0]);
            res = 
            code[0] = sd + (code2[0] - '0') + carry + '0';
        }
        code[n] = '\0';
        if (carry > 0) {
            char* new_code = calloc((n + 1), sizeof(char));
            new_code[0] = carry + '0';
            for (i = 0; i < n; i++) {
                new_code[i+1] = code[i];
            }
            new_code[n+1] = '\0';
            printf("\tcode %s\n", new_code);
            return new_code;
        }
        printf("\tcode %s\n", code);
        return code;
    }

    for (i = n - 1; i > len2; i--) {
        code[i] = code1[i];
        printf("code[i]: %c\n", (char)code1[i]);
    }
    for (i = len2; i > 0; i--) {
        printf("i: %lld\n", i);
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
        printf("\tdigit code %c\n", (char)code[i]);
        if (dec > 0) {
            carry = dec;
        } else {
            carry = 0;
        }
        printf("\tcarry %d\n", dec);
        if (carry > 0 && i == 1) {
            printf("need extra first digit\n");
            res = (sd + carry)%8;
            printf("res: %d\n", res);
            code[0] = res + '0';
            printf("\tdigit code %c\n", (char)code[0]);
            carry = (sd + carry)/8;
            printf("carry: %d\n", carry);
        }
    } 
    code[n] = '\0';
    if (carry > 0) {
        char* new_code = calloc((n + 1), sizeof(char));
        new_code[0] = carry + '0';
        for (i = 0; i < n; i++) {
            new_code[i+1] = code[i];
        }
        new_code[n+1] = '\0';
        printf("\tcode %s\n", new_code);
        return new_code;
    }
    printf("\tcode %s\n", code);
    return code;
}

// Функция перевода из восьмиричного числа, записанного 
// в строку code, в двоичную систему счисления.
// Возвращает строку res с двоичной записью числа.
// map[] - это массив, где элемент соответствует двоичной строке индекса
// т.е. map[0] = "000", map[1] = "001", ... , map[7] = "111".
char* convert_from_octal_to_binary(char* code, char* map[]) {
    char* res;
    int len = strlen(code);
    res = calloc(len * 3 + len, sizeof(char));
    strcpy(res, "");
    for (int i = 0; i < len; i++) {
        printf("%d - %c\n", i, (char)code[i]);
        int c = code[i] - '0';
        printf("oct: %d; bin: %s\n", c, map[c]);
        strncat(res, map[c], 3);
        printf("cur res: %s\n", res);
    }
    res[strlen(res) + 1] = '\0';
    printf("bin_res: %s\n", res);
    return res;
}

// Функция перевода двоичных строк длинной до 32 символов
// в макро-цифры в системе счисления по основанию 2^32.
// Аргументом функции является строка code длинной <= 32 символа.
// Функция возвращает число res в системе по основанию 2^32.
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

// Функция печати макро-цифр в системе 2^32 
// в файл (если указан аргумент не null), иначе в стандартный поток вывода
void print_Link(LINK* l, FILE* output){
    // printf("\nis link null:%u\n", l == NULL);
    if (output != NULL) {
        if (l != NULL) {
            char* digit = calloc(30, sizeof(char));
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

// Функция печати макро-цифр в системе 2^64 
// в файл (если указан аргумент не null), иначе в стандартный поток вывода
void print_Link_64(LINK* l, FILE* output) {
    if (output != NULL) {
        if (l != NULL && PREV(l) != NULL) {
            unsigned long senior = l->pair.n;
            unsigned long junior = l->prec->pair.n;
            char* macrodigit = calloc(60, sizeof(char));
            char* jun = calloc(30, sizeof(char));
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
            char* macrodigit = calloc(60, sizeof(char));
            char* jun = calloc(30, sizeof(char));
            sprintf(macrodigit, "%ld", senior);
            sprintf(jun, "%ld", junior);
            strcat(macrodigit, jun);
            printf("%s",macrodigit);
            print_Link_64(l->prec->prec, NULL);
        }
    }
}

// Функция перевода числа из 2^64 в 2-ю с.с
// На вход структура LINK последней макро-цифры, функция возвращает бинарную запись   
char* convert_to_binary(LINK* l) {
    char* res; // бинарная запись числа в 2^64
    char* b; // вспомогательная строка для записи остатков от деления на 2
    char* buffer; //  вспомогательный буфер для перевыделения памяти 
    int i = 0; //счетчик
    unsigned long long length = 1; // длина бинарной строки
    res = calloc(2, sizeof(char));
    strcpy(res, "");
    unsigned long digit; // цифра из структуры LINK
    int k = 0; // счетчик итераций
    while (l != NULL ) {
        printf("\nconvert_to_binary_>: the start of iteration, was_res = %p\n",res); // getchar();
        buffer = (char*)realloc(res, length + 32); // перевыделяем память для очередной макро-цифры 2^32
        if (buffer == NULL) {
            perror("Error: realloc memory for binary in convert_to_binary().");
        }
        res = buffer;
        printf("\nconvert_to_binary _>: res = %p\n",res); // getchar();
        digit = l->pair.n;
        
        printf("\nconvert_to_binary _>: iteration %d\n",k++); // getchar();
        printf("\nconvert_to_binary _>: digit is: %ld\n", digit);
        //переводим макро-цифру в 2-ю систему
        if (digit == 0) {
            strcat(res, "0");
            length++;
            res[length + 1] = '\0';
        } else {
            // если не ноль, берем остатки от деления на 2
            b = (char*)calloc(33, sizeof(char));
            for (i = 0; digit > 0; i++) {    
                b[i]= (digit % 2) + '0';    
                printf("digit: %ld, ost: %c\n", digit, (char)b[i]); 
                digit = digit / 2;   
            }
            b[i] = '\0';
            printf("b: %s\n", b);
            long s = strlen(b);
            // переворачиваем последовательность остатков от деления на 2
            char* binary = (char*)calloc((s + 1), sizeof(char));
            binary[s] = '\0';
            for (i = 0; i < s; i++) {
                binary[i] = b[s - 1 - i];
            } 
            free(b);
            printf("current binary is: %s\n", binary);
            long long len = strlen(binary);
            //при необходиимости добавляем незначащие нули вначало
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
            // добавляем к общей бинарной записи
            strncat(res, binary, len);
            length += len;
            res[length + 1] = '\0';
        }   
        printf("current res: %s\n", res);
        // переходим к следующей макро-цифре
        l = l->foll;
        printf("\nconvert_to_binary_>: the end of the iteration, l = %p\n",l);  //getchar();
    }
    return res;
}

// Вспомогательная функция для перевода бинарной записи цифры в восьмиричную систему.
// Возвращает число res с восьмиричной записью числа.
// map[] - это массив, где элемент соответствует двоичной строке индекса
// т.е. map[0] = "000", map[1] = "001", ... , map[7] = "111".
int octal_digit(char* map[], char* bin) {
    int res = -1;
    for (int i = 0; i < 8; i++) {
        if (strcmp(bin, map[i]) == 0) return i;
    }
    return res;
}

// Функция перевода числа, записанного в строке code из 2-й в 8-ю с.с 
// Аргумент map[] - это массив, где элемент соответствует двоичной строке индекса
// т.е. map[0] = "000", map[1] = "001", ... , map[7] = "111".
// Используется вспомогательная функция octal_digit().
char* convert_from_binary_to_octal(char* code, char* map[]) {
    long bin_len = strlen(code), len; // длина бинарной записи
    len = bin_len / 3; // длина восьмиричной записи
    printf("octal len: %ld\n", len);
    char* octal = calloc(len + 1, sizeof(char)); // восьмиричная запись наоборот
    char* res; // восьмиричная запись
    printf("cur res: %s\n", octal);
    long j = 0;
    int d;
    char bin[3];
    for (long i = bin_len; i > 0; i -= 3) {
        printf("i: %ld\n", i);
        slice(code, bin, i - 3, i);
        d = octal_digit(map, bin);
        printf("%s - %d\n", bin, d);
        char digit[2];
        sprintf(digit, "%d", d);
        octal[j] = (digit[0] -'0') + '0';
        j++;
        printf("cur res: %s\n", octal);
    }
    res = my_reverse(octal);
    res[j] = '\0';
    printf("res: %s\n", res);
    return res;
}

char* my_decimal_sub(char* code1, char* code2, long n) {
    char *code;
    int digit, digit1, digit2, sd, dec, res, carry = 0;
    code = calloc(n + 1, sizeof(char));
    printf("\nDEC SUB: first: %s\n", code1);
    printf("DEC SUB: second: %s\n", code2);
    sd = code1[0] - '0';
    printf("\tmy_decimal_sub_>: first digit from start: %d\n", sd);  
    code[0] = sd + '0';
    for (int i = n - 1; i > 0; i--) {
        digit1 = code1[i]-'0'; 
        digit2 = code2[i]-'0'; 
        printf("\tmy_decimal_sub_>: first digit: %d\n", digit1);
        printf("\tmy_decimal_sub_>: second digit: %d\n", digit2);
        digit = digit1 - digit2 - carry;
        if (digit < 0) {
            carry = 1;
            code[i] = (digit + 10) + '0';
        } else {
            carry = 0;
            code[i] = digit + '0';
        }
        printf("\tmy_decimal_sub_>: digit code %c\n", (char)code[i]);
        if (carry != 0 && i == 1) {
            code[0] = sd - carry + '0';
        }
    } 
    code[n] = '\0';
    printf("\tmy_decimal_sub_>: code %s\n", code);
    return code;
}

void generate_test_number(unsigned long long len){
    FILE* f = fopen("../tests/test.txt", "w");
    srand(time(NULL));
    for (unsigned long long i = 0; i < len; i++) {
        fprintf(f, "%d", (rand() % 9) + 1);
    }
    fclose(f);
    printf("tests/test.txt is created!\n");
}

int main() {
    generate_test_number(3);
    clock_t t;  // Переменная для отсчета времени выполнения алгоритмов.
    t = clock();
    FILE * f = fopen("../tests/test.txt", "rt"); // f - указатель на тестовый файл.
    char *code; // Строка для хранения десятичной записи числа.
    char *code1; // Вспомогательная строка для хранения десятичной записи числа и преобразования в восьмиричную запись
    size_t n = 0, m = 1;
    int c; // переменная для посимвольного считывания их файла
    LINK* start_number = (LINK*)malloc(sizeof(LINK)); // первая макро-цифра
    LINK* end_number;  // последняя макро-цифра
    LINK* prev_number; // предыдущая макро-цифра - вспомогательная переменная
    prev_number = NULL;
    bool flag_chetn = false; // флаг четности для заполнения структур link в смысле 2^64
    unsigned long long old_len, len; // old_len - длина бинарной записи числа, len - длина бинарной записис числа - вспомогательная


    if (f == NULL) {
        perror("Error: There is no such file or file has an error.");
        return(-1);
    }
    // Установка указателя положения в файле на конец файла
    fseek(f, 0, SEEK_END);
    // ftell(f) возвращает текущее значение указателя положения в файле 
    // Таким образом, получаем длину файла = длине десятичного числа 
    unsigned long f_size = ftell(f); // Длина входного десятичного числа
    // Установка указателя положения в файле на начало файла
    fseek(f, 0, SEEK_SET);

    code = malloc(f_size + 1);
    code1 = malloc(f_size + 2);
    code1[0] = '0';
    while ((c = fgetc(f)) != EOF) {
        code[n++] = (char)c;
        code1[m++] = (char)c;
        // printf("char: %c\n", (char)c);
    }

    code[n] = '\0'; 
    code1[m] = '\0'; 
    unsigned long cursor = 0;
    unsigned long long ll = 0, l = 0;
    bool flag = false;
    for (unsigned long i = 1; i < f_size; i++) {
        cursor = i + 1;
        char* todigits;
        todigits = calloc(f_size + 1, sizeof(char));
        if (l > ll) {
            ll = l;
            flag = true;
            cursor += 1;
            char* help = (char*)calloc((l + 1), sizeof(char));
            memcpy(help, code1, l + 1);
            printf("help: %s\n", help);
            code1 = (char*)realloc(code1, l + 1);
            memcpy(code1, help, l + 1);
            free(help);
            slice(code1, todigits, 0, cursor - 1);
            todigits[cursor - 1] = '\0';
        } else if (flag) {
            cursor += 1;
            slice(code1, todigits, 0, cursor - 1);
            todigits[cursor - 1] = '\0';
        } else {
            slice(code1, todigits, 1, cursor);
            todigits[cursor] = '\0';
        }
        printf("cursor: %ld\n", cursor);
        
        printf("code: %s\n", code1);
        printf("tidigits: %s\n", todigits);
        if (code1[0] != '0') {
            ll = strlen(code1);
        } else {
            ll = strlen(code1) - 1;
        }
        printf("old: %lld, new: %lld\n", ll, l);
        char* tmp = calloc(f_size + 1, sizeof(char));
        tmp = mul_x_2_octal(todigits, strlen(todigits));
        printf("tmp: %s\n", tmp);
        if (flag) {
            while (tmp[0] == '0') tmp++;
        }
        char* adds;
        adds = my_octal_add(code1, tmp, f_size + 1);
        memcpy(code1, adds, strlen(adds));
        free(adds);
        if (code1[0] != '0') {
            l = strlen(code1);
        } else {
            l = strlen(code1) - 1;
        }
        free(todigits);
        printf("old: %lld, new: %lld\n", ll, l);
    }
    fclose(f);
    printf("\ndecimal number: %s\n", code);
    if (code1[0] == '0') {
        code1++;
    }
    printf("octal number: %s\n", code1);
    FILE * o = fopen("output.txt", "w");
    fputs("===IN===\ndecimal: ", o);
    fputs(code, o);
    free(code);
    fputs("\noctal: ", o);
    fputs(code1, o);
    // массив для перевода 8 -> 2
    // каждому индексу(цифре 8 с.с) соотвествует ее бинарная запись
    char* map[] = { "000", "001", "010", "011", "100", "101", "110", "111"};

    // перевод 8 -> 2
    char* binary;
    binary = convert_from_octal_to_binary(code1, map);
    // free(code1);
    printf("binary: %s\n", binary);
    fputs("\nbinary: ", o);
    fputs(binary, o);
    fputs("\n", o);

    // перевод 2 -> 2^32
    printf("2^32:\n");
    fputs("\n\n2^32:\n", o);
    len = strlen(binary);
    // идем по строке (бинарной записи) числа, отщепляя с конца подстроки по 32 символа,
    // поэтому макро-цифры записываются в порядке справа налево, т.е.
    //              PREV
    //              --->
    // macro-digit2 <--- macro-digit1
    //              NEXT
    while (len > 0) {
        // printf("\nlen: %lld\n", len);
        LINK* number; // очередная макро-цифра
        number = (LINK*)malloc(sizeof(LINK));
        // если предыдущая макро-цифра была, значит создаем связь NEXT
        if (prev_number != NULL) {
            NEXT(prev_number) = number;
        }
        // тип digit - 'd'
        TYPE(number) = 'd';
        // создаем связь PREV
        PREV(number) = prev_number;
        char* str = calloc(33, sizeof(char)); // строка в 32 символа
        old_len = len;
        // если бинарная строка больше 32 символов или 32 символа
        if (len >= 32) {
            slice(binary, str, len-32, len);
            len -= 32;
        // если бинарная строка меньше 32 символов
        } else {
            slice(binary, str, 0, len);
            len -= len;
        }
        printf("str: %s\n", str);
        // записываем макро-цифру в поле pair.n
        // макро-цифра вычисляется с помощью перевода 2->2^32
        PAIR(number).n = convert_to_2_in_32_system(str);
        free(str);
        
        // сохраняем последнюю (первую с конца бинарной записи) макро-цифру,
        // это необходимо для дальнейшего перевода 2^32 -> 2
        if (old_len == strlen(binary)) {
            end_number = number;
        }
        prev_number = number;
        // меняем флаг четности
        flag_chetn = !flag_chetn;
    }
    // когда вся бинарная строка пройдена, делаем проверку. нужен ли незначащий ноль
    // если нужен - добавляем
    // отмечаем первую (последнюю с конца бинарной строки) макро-цифру
    // это необходимо для функций print_Link, print_Link_64
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
        NEXT(start_number) = NULL;
    }
    free(binary);
    // печать макро-цифр 2^32
    print_Link(start_number, NULL);
    print_Link(start_number, o);
    // печать макро-цифр 2^64
    printf("\n2^64:\n");
    fputs("\n2^64:\n", o);
    print_Link_64(start_number, NULL);
    print_Link_64(start_number, o);
    
    l = 0;

    // отмечаем время выполнения алгоритма
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("\nin algorithm took %f seconds to execute \n", time_taken);
    fputs("\nin algorithm took ", o);
    char time_arr[100];
    sprintf(time_arr, "%2.7f", time_taken);
    fputs(time_arr, o);
    fputs(" seconds to execute \n", o);


    printf("\n==== OUT ALGORITHM ====\n");
    t = clock();
    // печать макро-цифр 2^64
    fputs("\n===OUT===\n2^64:\n", o);
    print_Link_64(start_number, o);
    // печать макро-цифр 2^32
    fputs("\n\n2^32:\n", o);
    print_Link(start_number, o);
    // free(code1);

    // перевод из 2^64 в 2
    char* binary2;
    binary2 = convert_to_binary(end_number);
    // добавление незначащих нулей
    l = strlen(binary2);
    while (l % 3 != 0) {
       binary2[l++] = '0';
       binary2[l++] = '\0';
       l = strlen(binary2);
    }
    char* result;
    result = my_reverse(binary2);
    free(binary2);
    printf("binary: %s\n", result);
    fputs("\nbinary: ", o);
    fputs(result, o);

    // перевод из 2 в 8
    char* octal;
    octal = convert_from_binary_to_octal(result, map);
    while (octal[0] == '0') {
        octal++;
    }
    printf("octal: %s\n", octal);
    fputs("\noctal: ", o);
    fputs(octal, o);
    size_t size = strlen(octal);

    // перевод из 8 в 10
    for (int i = 0; i < size; i++) {
        cursor = i + 1;
        if (cursor == size) {
            break;
        }
        // printf("\ncurrent digit char: %c\n", octal[i]);
        char* todigits;
        todigits = calloc(size + 1, sizeof(char));
        slice(octal, todigits, 0, cursor);
        todigits[cursor] = '\0';
        // printf("cur: %s\n", todigits);
        char* tmp;
        tmp = calloc(size + 1, sizeof(char));
        tmp = mul_x_2(todigits, size - 1);
        // printf("tmp: %s\n", tmp);
        char* subs;
        subs = my_decimal_sub(octal, tmp, size);
        subs[size + 1] = '\0';
        memcpy(octal, subs, size + 1);
        free(subs);
        free(todigits);
    }
    char* dec = (char*)calloc((size + 1), sizeof(char));
    memcpy(dec, octal, size + 1);
    while (dec[0] == '0') {
        dec++;
    }
    printf("decimal: %s\n", dec);
    fputs("\ndecimal: ", o);
    fputs(dec, o);
    // free(dec);
    // free(octal);
    
    // отмечаем время выполнения алгоритма 2^64 -> 10 
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("\nout algorithm took %f seconds to execute \n", time_taken);
    fputs("\nout algorithm took ", o);
    sprintf(time_arr, "%2.7f", time_taken);
    fputs(time_arr, o);
    fputs(" seconds to execute \n", o);
    free(start_number);
    fclose(o);
    return 0;
}
