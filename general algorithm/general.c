#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>


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


// Вспомогательная функция получения длины строки, 
// с проверкой, что строка состоит только из цифр.
unsigned int my_strlen(const char *s) {
    unsigned int count = 0;
    while(*s!='\0') {
        if (isdigit(*s)) count++;
        s++;
    }
    return count;
}


// Вспомогательная функция, возвращает перевернутую строку.
char* my_reverse(char* code) {
    long s = strlen(code);
    char* result = malloc(s + 1);
    for (int i = 0; i < s; i++) {
        result[i] = code[s - 1 - i];
    }
    result[s] = '\0';
    return result;
}


// Функция стандартного алгоритма перевода десятичного числа, 
// записанного в строку, в восьмиричную систему счисления.
// Возвращает строку с восьмиричной записью числа.
// алгоритм деления на 8 столбиком и сохранение остатков от деления
char* convert_from_decimal_to_octal(char* code, long n) {
    bool new_step = false; // флаг о том, что поменялось делимое (т.е один остаток найден, продолжаем деление)
    char* result; // восьмириная запись числа
    result = (char*)malloc((n + 2) * sizeof(char));
    int j = 0, d; //j - счетчик длины восьмиричного числа, d - очередная десятичная цифра
    char* code2; // вспомогательная строка для записи частных при делении на 8
    char* first_two_digits; // строка для записи очередных двух чисел десятичного числа при делении столбиком
    first_two_digits = malloc(3);
    long digits, rem, quot, len = n; // digits - очередные две цифры, rem - остаток от деления, quot - частное, len - длина десятичного числа
    int first_digit = code[0] - '0', flag = 0; // first_digit - первая цифра десятичного числа, flag - флаг, о виде делимого (если = 0, значит цифра 8 или 9, если =1, значит меньше 8 и нужно брать две цифры) 
    // если первая цифра меньше 8, значит берем первые две цифры и делим на 8
    // если 8  или 9, делим на 8
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
        quot = first_digit / 8; 
        rem = first_digit - quot * 8; 
        printf("quot and rem: %ld, %ld\n", quot, rem);
    }
    // пока делимое не будет однозначным
    while( len != 1) {   
        // если найден остаток и поменялось делимое (началась новая итерация),
        // делаем то же самое с проверкй первой цифры (первых двух цифр) 
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
                free(code2);
            } else {
                printf("first 1 digit: %d\n", first_digit);
                quot = first_digit / 8; // 1;
                rem = first_digit - quot * 8; //0;
                printf("quot and rem: %ld, %ld\n", quot, rem);
                flag = 0;
            }
        }
        int k = 0; // счетчик для строки частного
        code2 = malloc(len + 1);
        code2[k++] = (int)quot + '0'; //записали частное от деления первой цифры(первых двух цифр)
        // продолжаем процесс деления столбиком
        for (int i = 1; i < len; i++) {
            printf("i: %d\n", i);
            // если были использованы две цифры, а не одна, увеличиваем счетчик i
            if (flag == 1) {
                i++;
                if (i == len) {
                    // если вдруг конец числа
                    break;
                }
            } 
            d = code[i] - '0';
            printf("next digit: %d\n", d);
            printf("check quot and rem: %ld, %ld\n", quot, rem);
            flag = 0;
            // если остаток от предыдущего деления не ноль, значит спускаем следующую цифру
            if (rem != 0) {
                digits = rem * 10 + d;
                printf(" next 2 digits: %ld\n", digits);
                quot = digits / 8;
                rem = digits - quot * 8;
                printf(" quot and rem: %ld, %ld\n", quot, rem);
            // иначе проверяем брать одну следующую цифоу или две
            } else {
                if (d < 8 && i != len - 1) {
                    code2[k++] = '0';
                    slice(code, first_two_digits, i, i + 2);
                    first_two_digits[2] = '\0';
                    digits = atol(first_two_digits);
                    printf(" next 2 digits: %ld\n", digits);
                    quot = digits / 8;
                    rem = digits - quot * 8;
                    printf(" quot and rem: %ld, %ld\n", quot, rem);
                    i++;
                } else {
                    printf(" next 1 digit: %d\n", d);
                    quot = d / 8; 
                    rem = d - quot * 8;
                    printf(" quot and rem: %ld, %ld\n", quot, rem); 
                }  
            }
            code2[k++] = (int)quot + '0';
        }
        // приписываем полученный на итерации остаток
        result[j++] =  (int)rem + '0';
        new_step = true;
        printf("current res: %s\n\n", result);
        code2[k] = '\0';
        printf("k: %d\n", k);
        // меняем делимое (старое частное -> новое делимое)
        memcpy(code, code2, k);
        printf("---current number: %s---\n", code2);
        printf("old len: %ld\n", len);
        len = k;
        printf("len: %ld\n", len);
              
    }
    free(first_two_digits);

    // когда делимое стало однозначным
    if (len == 1) {
        d = code[0] - '0';
        while (d != 0) {
            printf("last digit: %d\n", d);
            if (d < 8) {
                result[j++] = d +'0';   
            } else {
                quot = d / 8;
                rem = d % 8;
                printf("quot and rem: %ld, %ld\n", quot, rem);
                result[j++] = (int)rem + '0';
            }
            d = d/8;
            printf("current res: %s\n\n", result);
        }
    }
    // перевернутая восьмиричная запись 
    result[j++] =  '\0';
    printf("res: %s\n\n", result);
    free(code2);
    return result;
}


// Функция перевода из восьмиричного числа, записанного 
// в строку code, в двоичную систему счисления.
// Возвращает строку res с двоичной записью числа.
// map[] - это массив, где элемент соответствует двоичной строке индекса
// т.е. map[0] = "000", map[1] = "001", ... , map[7] = "111".
char* convert_from_octal_to_binary(char* code, char* map[]) {
    char* res;
    int len = strlen(code);
    res = malloc(len * 3 + 1);
    strcpy(res, "");
    for (int i = 0; i < len; i++) {
        printf("%d - %c\n", i, (char)code[i]);
        int c = code[i] - '0';
        printf("oct: %d; bin: %s\n", c, map[c]);
        strncat(res, map[c], 3);
        printf("cur res: %s\n", res);
    }
    res[len * 3 + 1] = '\0';
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

// Функция печати макро-цифр в системе 2^64 
// в файл (если указан аргумент не null), иначе в стандартный поток вывода
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

// Функция перевода числа из 2^64 в 2-ю с.с
// На вход структура LINK последней макро-цифры, функция возвращает бинарную запись   
char* convert_to_binary(LINK* l) {
    char* res; // бинарная запись числа в 2^64
    char* b; // вспомогательная строка для записи остатков от деления на 2
    char* buffer; //  вспомогательный буфер для перевыделения памяти 
    int i = 0; //счетчик
    unsigned long long length = 1; // длина бинарной строки
    res = malloc(1 * sizeof(char));
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
            b = (char*)malloc(33 * sizeof(char));
            for (i = 0; digit > 0; i++) {    
                b[i]= (digit % 2) + '0';    
                printf("digit: %ld, ost: %c\n", digit, (char)b[i]); 
                digit = digit / 2;   
            }
            b[i] = '\0';
            printf("b: %s\n", b);
            long s = strlen(b);
            // переворачиваем последовательность остатков от деления на 2
            char* binary = (char*)malloc((s + 1)* sizeof(char));
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
        help = (char*)calloc((len1 + 1), sizeof(char));
        help[len1] = '\0';
        for (i = 0; i < len; i++) {
            help[i] = '0';
        }
        for (i = len, j = 0; i < len2 + len; i++, j++) {
            help[i] = num2[j];
        }
        num_2 = (char*)calloc((len1 + 1),sizeof(char));
        num_2[len1] = '\0';
        for (i = 0; i < len1; i++) {
            num_2[i] = help[i];
        }
        num_1 =(char*)calloc((len1 + 1), sizeof(char));
        num_1[len1] = '\0';
        for (i = 0; i < len1; i++) {
            num_1[i] = num1[i];
        }
        free(help);
    } else if (len2 > len1) {
        len = len2 - len1;
        help = (char*)calloc((len2 + 1), sizeof(char));
        help[len2] = '\0';
        for (i = 0; i < len; i++) {
            help[i] = '0';
        }
        for (i = len, j = 0; i < len1 + len; i++, j++) {
            help[i] = num1[j];
        }
        num_1 = (char*)calloc((len2 + 1),sizeof(char));
        num_1[len2] = '\0';
        for (i = 0; i < len2; i++) {
            num_1[i] = help[i];
        }    
        num_2 =(char*)calloc((len2 + 1),sizeof(char));
        num_2[len2] = '\0';
        for (i = 0; i < len2; i++) {
            num_2[i] = num2[i];
        }
        free(help);
    } else {
        num_1 = (char*)calloc((len1 + 1), sizeof(char));
        num_1[len1] = '\0';
        num_2 = (char*)calloc((len2 + 1), sizeof(char));
        num_2[len2] = '\0';
        for (i = 0; i < len1; i++) {
            num_1[i] = num1[i];
        }
        for (i = 0; i < len2; i++) {
            num_2[i] = num2[i];
        }
    }
    len = strlen(num_1);
    char* res = (char*)calloc((len + 2), sizeof(char));
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
        help = (char*)calloc((len + 1), sizeof(char));
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
    // generate_test_number(40);
    clock_t t;  // Переменная для отсчета времени выполнения алгоритмов.
    t = clock();
    FILE * f = fopen("../tests/test.txt", "rt"); // f - указатель на тестовый файл.
    char *code; // Строка для хранения десятичной записи числа.
    char *res; // Вспомогательная строка для получения перевернутой восьмиричной записи числа.
    char *code2; // Вспомогательная строка для хранения десятичной записи числа.
    char* binary; // Строка для хранения двоичной записи числа
    size_t n = 0, m = 0; // Счетчики для считывания файла и заполнения строк code и code2.
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

    printf("==== IN ALGORITHM ====\n");
    printf("size of file: %ld\n", f_size);
    
    code = malloc(f_size + 1);
    code2 = malloc(f_size + 1);
    while ((c = fgetc(f)) != EOF) {
        code[n++] = code2[m++] = (char)c;
    }
    code[n] = code2[m] = '\0';
    fclose(f);

    // перевод 10 -> 8
    res = convert_from_decimal_to_octal(code2, f_size);
    long s = strlen(res);
    // поскольку res - перевернутая запись, делаем реверс
    char* result = my_reverse(res);
    free(res);
    free(code2);
    result[(strlen(result) + 1)] = '\0';

    // запись в файл и лог 10 и 8
    printf("\ndecimal number: %s\n", code);
    printf("octal number: %s\n", result);
    FILE * o = fopen("output.txt", "w");
    fputs("===IN===\ndecimal: ", o);
    fputs(code, o);
    free(code);
    fputs("\noctal: ", o);
    fputs(result, o);
    
    // массив для перевода 8 -> 2
    // каждому индексу(цифре 8 с.с) соотвествует ее бинарная запись
    char* map[] = {"000", "001", "010", "011", "100", "101", "110", "111"}; 
    // перевод 8 -> 2
    binary = convert_from_octal_to_binary(result, map);
    free(result);
    // запись в файл и лог 2
    printf("\nbinary: %s\n", binary);
    len = strlen(binary);
    printf("len: %lld\n", len);
    binary[(strlen(binary) + 1)] = '\0';
    fputs("\nbinary: ", o);
    fputs(binary, o);

    // перевод 2 -> 2^32
    printf("2^32:\n");
    fputs("\n\n2^32:\n", o);

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
        char* str = malloc(33 * sizeof(char)); // строка в 32 символа
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
    // печать макро-цифр 2^64
    fputs("\n===OUT===\n2^64:\n", o);
    print_Link_64(start_number, o);
    // печать макро-цифр 2^32
    fputs("\n\n2^32:\n", o);
    print_Link(start_number, o);
    t = clock();
    // перевод из 2^64 в 2
    char* binary2;
    binary2 = convert_to_binary(end_number);
    printf("\nreversed binary: %s\n", binary2);
    // добавление незначащих нулей
    unsigned long long l = strlen(binary2);
    while (l % 3 != 0) {
       binary2[l++] = '0';
       binary2[l++] = '\0';
       l = strlen(binary2);
    }
    code2 = my_reverse(binary2);
    fputs("\nbinary: ", o);
    printf("binary: %s\n", code2);
    fputs(code2, o);
    // перевод из 2 в 8
    char* octal;
    octal = convert_from_binary_to_octal(code2, map);
    free(code2);
    printf("octal: %s\n", octal);
    fputs("\noctal: ", o);
    fputs(octal, o);
    size_t size = strlen(octal);
    printf("\nsize of octal: %ld\n\n", size);
    int deg = 0, d = 0; // deg - степень, d - текущая цифра
    // перевод из 8 в 10
    // char* dec = calloc((size + 1), sizeof(char));
    // char* cur;
    // char* pow;
    // for (int i = size - 1; i >= 0; i--) {
    //     d = octal[i] - '0';
    //     printf("\n\td: %d\n", d);
    //     pow = my_pow(8, deg);
    //     printf("\tpow(8, deg): %s\n", pow);

    //     char* num = large_mul_digit(d, pow);

    //     printf("\tnum: %s\n", num);
    //     ++deg;
    //     cur = large_addition(dec, num); 
    //     memcpy(dec, cur, strlen(cur) + 1);
    //     printf("\t\tcurrent res: %s\n", dec);

    // }
    // dec[strlen(dec) + 1] = '\0';
    // printf("decimal: %s\n", dec);
    // fputs("\ndecimal: ", o);
    // fputs(dec, o);
    // free(octal);
    // free(dec);
    // отмечаем время выполнения алгоритма 2664 -
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("\nout algorithm took %f seconds to execute \n", time_taken);
    fputs("\nout algorithm took ", o);
    sprintf(time_arr, "%2.7f", time_taken);
    fputs(time_arr, o);
    fputs(" seconds to execute \n", o);
    fclose(o);
    free(start_number);
    return 0;
}
