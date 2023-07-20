#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main()
{
    long temp = 0;
    int sum = 0;
    FILE * f = fopen("../tests/test1.txt", "rt");
    if( f == NULL ) {
        perror("Error: ");
        return(-1);
    }
    while ( fscanf(f, "%ld", &temp) == 1 ) {
        sum += temp;
    }
        
    fclose(f);
    
    FILE * o = fopen("output.txt", "w");
    fprintf(o, "%d", sum);
    fclose(o);
    return 0;
}