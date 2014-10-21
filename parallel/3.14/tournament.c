#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
//#include <threads.h>
#include <pthread.h>

#ifndef __STDC_NO_THREADS__
# error threads not supported
#endif

static bool stop = false;

static
volatile unsigned       m = 0u,
                        n = 0u,
                        * last = NULL;

static volatile bool    cont[] = {false, true};

static void * test_barrier(void * pp)
{
    unsigned    * p = (unsigned *)pp;
    size_t      sw = 0u;

    while ( ! stop )
    {
        int x = rand();
        for (int i = 2; i < x; ++i)
            if ( x % i == 0 )
            {
                break;
            }

        size_t  stage = 1u, 
                offset = 0u;

        while ( true )
        {
            last[offset + *p / (1 << stage)] = *p;
            while ( last[offset + *p / (1 << stage)] == *p && ! cont[sw] );

            if ( cont[sw] )
            {
                sw ^= 1;
                break;
            }

            printf("%2u's won, stage %zu\n", *p, stage);

            if ( stage == m )
            {
                puts(">>> NEXT ROUND <<<");

                cont[sw ^ 1] = false;
                cont[sw] = true;
                sw ^= 1;
                break;
            }

            stage++;
            offset += 1 << (m - stage + 1);
        }
    }
    free(p);
}

int main(int argc, char * argv[])
{
    if ( argc != 2 )
    {
        return EXIT_FAILURE;
    }

    if ( sscanf(argv[1], "%u", &m) != 1 )
    {
        return EXIT_FAILURE;
    }

    n = 1 << m;

    last = calloc(n, sizeof(unsigned));

    if ( last == NULL )
    {
        return EXIT_FAILURE;
    }

    //thrd_t          threads[n];
    pthread_t           threads[n];

    for ( size_t i = 0u;   i < n;   ++i )
    {
        unsigned    * p = malloc(sizeof i);

        if ( p == NULL )
        {
            return EXIT_FAILURE;
        }

        *p = i;

//        if ( thrd_success != thread_create(&threads[i], test_barrier, p) )
        if ( 0 != pthread_create(&threads[i], NULL, test_barrier, p) )
        {
            return EXIT_FAILURE; 
        }
    }

    char    c;

    do
    {
        c = getchar();
    }
    while ( c != ' ' );

    stop = true;

    for ( size_t i = 0u;   i < n;   ++i )
    {
//        if ( thrd_success != thread_join(&threads[i], NULL) )
        if ( 0 != pthread_join(threads[i], NULL) )
        {
            return EXIT_FAILURE;
        }
    }


    return EXIT_SUCCESS;
}
