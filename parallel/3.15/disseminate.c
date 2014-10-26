#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
//#include <threads.h>
#include <pthread.h>

static bool stop = false;

static
volatile unsigned       m = 0u,
                        n = 0u,
                        * arrive = NULL;

static void * test_barrier(void * pp)
{
    unsigned    * p = (unsigned *)pp;

    while ( ! stop )
    {
        int x = rand();
        for (int i = 2; i < x; ++i)
            if ( x % i == 0 )
            {
                break;
            }

        size_t  stage = 1u;

        for ( unsigned s = 0;   s < m;   ++s )
        {
            arrive[*p]++;

            unsigned q = (*p + (1 << s)) % n;

            while ( arrive[*p] > arrive[q] );
        }

        printf("%u next\n", *p);
    }
    free(p);

    return NULL;
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

    arrive = calloc(n, sizeof(unsigned));

    if ( arrive == NULL )
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
