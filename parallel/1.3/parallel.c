#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

double          ** a = NULL,
                ** b = NULL,
                ** c = NULL;

size_t          n,
                P;

void * worker(void * arg)
{
    size_t      p = (size_t)arg,
                first = p * n / P,
                last = first + n / P;

    for ( size_t i = first;   i < last;   ++i )
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

    return NULL;
}

int main(int argc, char * argv[])
{
    if ( sscanf(argv[1], "%zu", &P) != 1 )
    {
        return EXIT_FAILURE;
    }

    if ( sscanf(argv[2], "%zu", &n) != 1 )
    {
        return EXIT_FAILURE;
    }

    if ( n < P || n % P != 0u )
    {
        printf("P doesn't evenly divide n\n");
        return EXIT_FAILURE;
    }

    printf("Matrix size: %zu\nStripes count: %zu\n", n, P);

    a = malloc(n * sizeof(double *));                               assert( a != NULL );
    b = malloc(n * sizeof(double *));                               assert( b != NULL );
    c = malloc(n * sizeof(double *));                               assert( c != NULL );

    for ( size_t i = 0u;   i < n;   ++i )
    {
        a[i] = calloc(n,  sizeof(double));                          assert( a[i] != NULL );
        b[i] = calloc(n,  sizeof(double));                          assert( b[i] != NULL );
        c[i] = calloc(n,  sizeof(double));                          assert( c[i] != NULL );

        for ( size_t j = 0u;   j < n;   ++j )
        {
            a[i][j] = 1.0;
            b[i][j] = 1.0;
        }
    }

    pthread_t           * workers = calloc(P, sizeof(pthread_t));   assert( workers != NULL );

    for ( size_t i = 0u;   i < P;   ++i )
    {
        int ret = pthread_create(&workers[i], NULL, worker, (void *)i);

        assert( ret == 0 );
    }

    for ( size_t i = 0u;   i < P;   ++i )
    {
        int ret = pthread_join(workers[i], NULL);

        assert( ret == 0 );
    }

    return EXIT_SUCCESS;
}
