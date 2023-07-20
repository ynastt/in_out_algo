#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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