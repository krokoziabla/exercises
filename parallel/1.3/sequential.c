#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
    size_t              n;

    if ( sscanf(argv[1], "%zu", &n) != 1 )
    {
        return EXIT_FAILURE;
    }

    printf("Matrix size: %zu\n", n);

    double              ** a = malloc(n * sizeof(double *));    assert( a != NULL );
    double              ** b = malloc(n * sizeof(double *));    assert( b != NULL );
    double              ** c = malloc(n * sizeof(double *));    assert( c != NULL );

    for ( size_t i = 0u;   i < n;   ++i )
    {
        a[i] = malloc(n * sizeof(double));                      assert( a[i] != NULL );
        b[i] = malloc(n * sizeof(double));                      assert( b[i] != NULL );
        c[i] = malloc(n * sizeof(double));                      assert( c[i] != NULL );

        for ( size_t j = 0u;   j < n;   ++j )
        {
            a[i][j] = 1.0;
            b[i][j] = 1.0;
        }
    }

    for ( size_t i = 0u;   i < n;   ++i )
    {
        for ( size_t j = 0u;   j < n;   ++j )
        {
            c[i][j] = 0.0;

            for ( size_t k = 0u;   k < n;   ++k )
            {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    return EXIT_SUCCESS;
}
