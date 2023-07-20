#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main()
{
    int temp = 0, sum = 0;
    FILE * f = fopen("tests/test1.txt", "rt");
    while ( fscanf(f, "%d", &temp) == 1 )
        sum += temp;
    fclose(f);
    
    FILE * o = fopen("output.txt", "w");
    fprintf(o, "%d", sum);
    fclose(o);
    return 0;
}