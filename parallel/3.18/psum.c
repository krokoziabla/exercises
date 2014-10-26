#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

static size_t       n = 0u;
static volatile unsigned    * sum = NULL;
static volatile size_t      * avail = NULL;

static
void * process(void * pp)
{
    size_t      p = (size_t)pp,
                d = 1u;

    while ( d < n )
    {
        if ( p >= d )
        {
            while ( avail[p - d] != d );

            unsigned    left = sum[p - d];

            avail[p - d] = 0u;

            if ( p + d < n )
            {
                while ( avail[p] == d );
            }

            sum[p] += left;
        }
        else if ( p + d < n )
        {
            while ( avail[p] == d );
        }

        d <<= 1;
        avail[p] = d;
    }
    return NULL;
}

extern
int main(int argc, char * argv[])
{
    if ( sizeof(void *) != sizeof(size_t) )
    {
        return EXIT_FAILURE;
    }

    if ( argc != 2 )
    {
        return EXIT_FAILURE;
    }

    if ( sscanf(argv[1], "%zu", &n) != 1 )
    {
        return EXIT_FAILURE;
    }

    pthread_t       * threads = calloc(n, sizeof(pthread_t));

    sum = calloc(n, sizeof(unsigned));
    avail = calloc(n, sizeof(size_t));

    if ( threads == NULL )
    {
        return EXIT_FAILURE;
    }

    for ( size_t i = 0;   i < n;   ++i )
    {
        avail[i] = 1;
        sum[i] = i + 1;

        if ( pthread_create(threads + i, NULL, process, (void *)i) != 0 )
        {
            return EXIT_FAILURE;
        }

        printf("%u\t", sum[i]);
    }
    putchar('\n');

    for ( size_t i = 0;   i < n;   ++i )
    {
        if ( pthread_join(threads[i], NULL) != 0 )
        {
            return EXIT_FAILURE;
        }
        printf("%u\t", sum[i]);
    }
    putchar('\n');

    return EXIT_SUCCESS;
}
