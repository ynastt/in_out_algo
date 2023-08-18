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
        fprintf(f, "%d", (rand() % 9) + 1);
    }
    fclose(f);
    printf("tests/test.txt is created!\n");
}

// how to run the program:
// gcc program.c
// ./a.out num1 num2
int main(int argc, char *argv[]) {
    if (argc == 3) {
        FILE* o = fopen("result.txt", "w");
        fputs("Number of iterations: ", o);
        fputs(argv[1], o);
        fputs("\nLength of test numbers: ", o);
        fputs(argv[2], o);
        fputs("\n", o);
        int iters = atoi(argv[1]);
        int length = atoi(argv[2]);
        double sum1 = 0, sum2 = 0, sum_1 = 0, sum_2 = 0;
        for (int j = 1; j <= iters; j++) {
            char line[30];
            sprintf(line, "\n\n===iteration %d===", j);
            fputs(line, o);
            generate_test_number(length);
            // call general algorithm & write results in "general algorithm/output.txt"
            if (chdir("general algorithm") != 0) {
                perror("chdir() to /general algorithm failed");
            }
            printf("GENERAL ALGORITHM\n");
            system("sh build.sh");
            // read the output.txt & find result number for future comparison;
            regex_t rx, rx1, rxt1, rxt2, rg;
            int d1, d11, d2, d3, d4;
            d1 = regcomp(&rx, "octal: ", 0);
            d11 = regcomp(&rx1, "decimal: ", 0);
            d2 = regcomp(&rxt1, "in algorithm took [0-9]\\.[0-9]* seconds to execute", 0);
            d3 = regcomp(&rxt2, "out algorithm took [0-9]\\.[0-9]* seconds to execute", 0);
            d4 = regcomp(&rg, "===OUT===", 0);
            FILE *f; 
            char str[1000];
            char *estr;
            double t1 = 0, t2 = 0, t3 = 0, t4 = 0;
            int counter = 0, counter1 = 0;
            char* res1 = (char*)malloc((length + 1)* sizeof(char));
            char* time1 = (char*)malloc((iters + 46)* sizeof(char));
            char* res2 = (char*)malloc((length + 1)* sizeof(char));
            char* time2 = (char*)malloc((iters + 46)* sizeof(char));
            char* res3 = (char*)malloc((length + 1)* sizeof(char));
            char* time3 = (char*)malloc((iters + 47)* sizeof(char));
            char* res4 = (char*)malloc((length + 1)* sizeof(char));
            char* time4 = (char*)malloc((iters + 47)* sizeof(char));
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
                d1 = regexec(&rx, str, 0, NULL, 0);
                d11 = regexec(&rx1, str, 0, NULL, 0);
                d2 = regexec(&rxt1, str, 0, NULL, 0);
                d3 = regexec(&rxt2, str, 0, NULL, 0);
                d4 = regexec(&rg, str, 0, NULL, 0);
                if (d1 == 0 && counter == 0) {
                    counter = 1;
                    strncpy(res1, str + 7, strlen(str));
                } 
                if (d11 == 0 && counter1 == 0) {
                    counter1 = 1;
                    strncpy(res3, str + 9, strlen(str));
                } 
                if (d2 == 0) {
                    strncpy(time1, str + 18, 10);
                } 
                if (d4 == 0) {
                    while(1) {
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
                        d3 = regexec(&rxt2, str, 0, NULL, 0);
                        if (d3 == 0) {
                            strncpy(time3, str + 19, 10);
                            break;
                        }
                    }
                    break;
                }
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
            printf("\n\n\nNEW ALGORITHM\n");
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
                d1 = regexec(&rx, str, 0, NULL, 0);
                d11 = regexec(&rx1, str, 0, NULL, 0);
                d2 = regexec(&rxt1, str, 0, NULL, 0);
                d3 = regexec(&rxt2, str, 0, NULL, 0);
                d4 = regexec(&rg, str, 0, NULL, 0);
                if (d1 == 0) {
                    strncpy(res2, str + 7, strlen(str));
                }
                if (d11 == 0) {
                    counter1 = 1;
                    strncpy(res4, str + 9, strlen(str));
                }
                if (d2 == 0) {
                    strncpy(time2, str + 18, 10);
                }
                if (d4 == 0) {
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
                        d3 = regexec(&rxt2, str, 0, NULL, 0);
                        if (d3 == 0) {
                            strncpy(time4, str + 19, 10);
                            break;
                        }
                    }
                    break;
                }
            }
            if (fclose(f) == EOF)  {
                perror("Error: Closing output new algorithm\n");
            }
            if (chdir("..") != 0) {
                perror("chdir() to .. failed");
            }
            int verdict_in = strcmp(res1, res2);
            int verdict_out = strcmp(res3, res4);
            if (verdict_in != 0 || verdict_out != 0) {
                fputs("\nERROR: RESULTS ARE DIFFERENT", o);
                fputs("\n\nTEST:\n", o);
                FILE* t = fopen("tests/test.txt", "r");
                if (t == NULL) {
                    perror("Error: Opening test");
                    return(-1);
                }
                estr = fgets(str, sizeof(str), f);
                fputs("number: ", o);
                fputs(str, o);
                fputs("\n10 -> 2^64", o);
                fputs("\ngeneral algorithm result: ", o);
                fputs(res1, o);
                fputs("new algorithm result: ", o);
                fputs(res2, o);
                fputs("\n2^64 -> 10", o);
                fputs("\ngeneral algorithm result: ", o);
                fputs(res3, o);
                fputs("new algorithm result: ", o);
                fputs(res4, o);
                return(-1);
            } else {
                fputs("\nSUCCESS: RESULTS ARE OK", o);
                fputs("\n", o);
                // find execution time and write;
                fputs("\n10 -> 2^64", o);
                fputs("\ngeneral algorithm time: ", o);
                fputs(time1, o);
                t1 = strtod(time1, &time1);
                fputs("\nnew algorithm time: ", o);
                fputs(time2, o);
                t2 = strtod(time2, &time2);
                
                fputs("\n2^64 -> 10", o);
                fputs("\ngeneral algorithm time: ", o);
                fputs(time3, o);
                t3 = strtod(time3, &time3);
                fputs("\nnew algorithm time: ", o);
                fputs(time4, o);
                t4 = strtod(time4, &time4);
                // sum
                sum1 += t1;
                sum2 += t2;
                sum_1 += t3;
                sum_2 += t4;

            }
            free(res1);
            free(res2);
            free(res3);
            free(res4);
            free(time1);
            free(time2);
            free(time3);
            free(time4);
        }
        // average execution time
        
        fputs("\n\n\n\nAVERAGE EXECUTION TIME:\n", o);
        fputs("\n\t\t10 -> 2^64\n", o);
        fputs("general algorithm: ", o);
        char average[100];
        sprintf(average, "%f", sum1/iters);
        fputs(average, o);
        fputs("\nnew algorithm: ", o);
        sprintf(average, "%f", sum2/iters);
        fputs(average, o);
        fputs("\n\n\t\t2^64 -> 10\n", o);\
        fputs("general algorithm: ", o);
        sprintf(average, "%f", sum_1/iters);
        fputs(average, o);
        fputs("\nnew algorithm: ", o);
        sprintf(average, "%f", sum_2/iters);
        fputs(average, o);
        
    } else if( argc > 3 ) {
        printf("Too many arguments.\n");
        printf("Write next command:   ./a.out <Iters> <In>\n");
        printf("where:\n\t<Iters> -  number of iterations.\n\t<In> - length of generating test number;\n");
    } else {
        printf("Write next command:   ./a.out <Iters> <In>\n");
        printf("where:\n\t<Iters> -  number of iterations.\n\t<In> - length of generating test number;\n");
    }
    return 0;
}
