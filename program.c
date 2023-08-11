#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <regex.h>

void generate_test_number(unsigned long long len){
    char* number = (char*)malloc((len + 1) * sizeof(char));
    FILE* f = fopen("tests/test.txt", "w");
    srand(time(NULL));
    for (unsigned long long i = 0; i < len; i++) {
        fprintf(f, "%d", rand()%10);
    }
    fclose(f);
    printf("tests/test.txt is created!\n");
}

// how to run the program:
// gcc program.c
// ./a.out num1 num2
int main(int argc, char *argv[]) {
    if( argc == 3 ) {
        FILE* o = fopen("result.txt", "w");
        printf("Number of iterations: %s\n", argv[1]);
        printf("Length of generated test numbers: %s\n", argv[2]);
        fputs("Length of test numbers: ", o);
        fputs(argv[2], o);
        int iters = atoi(argv[1]);
        for (int j = 1; j <= iters; j++) {
            printf("===iteration %d===\n", j);
            generate_test_number(atoi(argv[2]));
            // call general algorithm & write results in "general algorithm/output.txt"
            if (chdir("general algorithm") != 0) {
                perror("chdir() to /general algorithm failed");
            }
            // printf("GENERAL ALGORITHM\n");
            system("sh build.sh");
            // read the output.txt & find result number for future comparison;
            regex_t rx;
            int d1;
            d1 = regcomp(&rx, "octal: ", 0);
            FILE *f; 
            char str[1000];
            char *estr;
            char* res1 = (char*)malloc((iters + 1)* sizeof(char));
            char* res2 = (char*)malloc((iters + 1)* sizeof(char));
            f = fopen("output.txt", "rt");
            if (f == NULL) {
                perror("Error: Opening output gen algorithm");
                return(-1);
            }
            while (1) {
                estr = fgets(str, sizeof(str), f);
                if (estr == NULL) {
                    if (feof(f) != 0) {  
                        printf("\nend of output.txt\n");
                        break;
                    } else {
                        perror("Error: Reading output gen algorithm");
                        break;
                    }
                }
                // printf("  %s", str);
                d1 = regexec(&rx, str, 0, NULL, 0);
                if (d1 == 0) {
                    // printf("Pattern matched\n");
                    strncpy(res1, str + 7, strlen(str));
                    // printf("%s\n", res1);
                    break;
                }
                // else if (d1 == REG_NOMATCH) {
                //     printf("Pattern not found\n");
                // }
            }
            if (fclose(f) == EOF)  {
                perror("Error: Closing output gen algorithm\n");
            }
            if (chdir("..") != 0) {
                perror("chdir() to .. failed");
            }

            // call new algorithm  & write results in "general algorithm/output.txt"
            if (chdir("new algorithm") != 0) {
                perror("chdir() to /new algorithm failed");
            }
            // printf("NEW ALGORITHM\n");
            system("sh build.sh");
            // read the output.txt & find result number for future comparison;
            f = fopen("output.txt", "rt");
            if (f == NULL) {
                perror("Error: Opening output new algorithm");
                return(-1);
            }
            while (1) {
                estr = fgets(str, sizeof(str), f);
                if (estr == NULL) {
                    if (feof(f) != 0) {  
                        printf("\nend of output.txt\n");
                        break;
                    } else {
                        perror("Error: Reading output new algorithm");
                        break;
                    }
                }
                // printf("  %s", str);
                d1 = regexec(&rx, str, 0, NULL, 0);
                if (d1 == 0) {
                    // printf("Pattern matched\n");
                    strncpy(res2, str + 7, strlen(str));
                    // printf("%s\n", res2);
                    break;
                }
                // else if (d1 == REG_NOMATCH) {
                //     printf("Pattern not found\n");
                // }
            }
            if (fclose(f) == EOF)  {
                perror("Error: Closing output new algorithm\n");
            }
            if (chdir("..") != 0) {
                perror("chdir() to .. failed");
            }
            int verdict = strcmp(res1, res2);
            if (verdict != 0) {
                fputs("\n\nERROR: RESULTS ARE DIFFERENT", o);
                fputs("\n\nTEST:\n", o);
                FILE* t = fopen("tests/test.txt", "r");
                if (t == NULL) {
                    perror("Error: Opening test");
                    return(-1);
                }
                estr = fgets(str, sizeof(str), f);
                fputs("number: ", o);
                fputs(str, o);
                fputs("\ngeneral algorithm result: ", o);
                fputs(res1, o);
                fputs("new algorithm result: ", o);
                fputs(res2, o);
                return(-1);
            } else {
                printf("everything is ok, continue");
                // find time taken and summarize;
            }
        }
    }
    else if( argc > 3 ) {
        printf("Too many arguments.\n");
        printf("Write next command:   ./a.out <Iters> <In>\n");
        printf("where:\n\t<Iters> -  number of iterations.\n\t<In> - length of generating test number;\n");
    } else {
        printf("Write next command:   ./a.out <Iters> <In>\n");
        printf("where:\n\t<Iters> -  number of iterations.\n\t<In> - length of generating test number;\n");
    }
    return 0;
}
