#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static size_t           n,
                        q = 0u,
                        tuples = 0u;

static size_t           * Q;

static
void find_queen()
{
    for ( size_t i = 0u;   i < n * n;   ++i )
    {
        int         xi = i % n,
                    yi = i / n;

        bool        queen = true;

        for ( size_t iq = 0u;   iq < q;   ++iq )
        {
            int     xq = Q[iq] % n,
                    yq = Q[iq] / n;

            if ( (xq == xi) || (yq == yi) || ((xq + yq) == (xi + yi)) || ((xq - yq) == (xi - yi)) )
            {
                queen = false;
                break;
            }
        }

        if ( queen && (q == 0u || i > Q[q - 1u]) )
        {
            Q[q++] = i;

            if ( q == n )
            {
                for ( size_t j = 0u;   j < n;   ++j )
                {
                    printf("%zu\t", Q[j]);
                }
                putchar('\n');

                ++tuples;
            }
            else
            {
                find_queen();
            }

            --q;
        }
    }
}

extern
int main(int argc, char * argv[])
{
    assert( argc == 2 );

    n = atoi(argv[1]);
    Q = calloc(n, sizeof(size_t));        assert( Q != NULL );

    find_queen();

    printf("Total queen tuples: %zu\n", tuples);

    return EXIT_SUCCESS;
}
