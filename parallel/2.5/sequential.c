#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern
int main(int argc, char * argv[])
{
    assert( argc == 3 );

    size_t          m = atoi(argv[1]),
                    n = atoi(argv[2]),
                    counter = 0u;

    assert( m != 0u );
    assert( n != 0u );

    unsigned        * a = calloc(m + 1u, sizeof(unsigned)),
                    * b = calloc(n + 1u, sizeof(unsigned));

    assert( a != NULL );
    assert( b != NULL );

    srand(time(NULL));

    a[0u] = rand() % 10;
    for ( size_t i = 0;   i < m;   ++i )
    {
        a[i + 1u] = a[i] + rand() % 10 + 1u;
        printf("\t%u", a[i]);
    }
    a[m] = UINT_MAX;
    putchar('\n');

    b[0u] = rand() % 10;
    for ( size_t i = 0;   i < n;   ++i )
    {
        b[i + 1u] = b[i] + rand() % 10 + 1u;
        printf("\t%u", b[i]);
    }
    b[n] = UINT_MAX;
    putchar('\n');

    while ( *a != UINT_MAX || *b != UINT_MAX )
    {
        if ( *a == * b )
        {
            ++counter;
            ++a;
            ++b;
        }
        else if ( *a < *b )
        {
            ++a;
        }
        else
        {
            ++b;
        }
    }

    printf("Common elements: %zu\n", counter);

    return EXIT_SUCCESS;
}
