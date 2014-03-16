#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static
void print_array(int array[], size_t size)
{
    for ( size_t i = 0u;   i < size;   ++i )
    {
        printf("%i\t", array[i]);
    }
    putchar('\n');
}

static
void quicksort(int * left, int * right)
{
    if ( right - left < 1 )
    {
        return;
    }
    // else right - left >= 2

    int         * m = left + rand() % (right - left + 1),
                * l = left,
                * r = right;

    while ( true )
    {
        while ( *l <= *m && l != m )
        {
            ++l;
        }

        while ( *r >= *m && r != m )
        {
            --r;
        }

        if ( l == r )
        {
            break;
        }

        *l = *l + *r;
        *r = *l - *r;
        *l = *l - *r;

        if ( l == m )
        {
            m = r;
            ++l;
        }
        else if ( r == m )
        {
            m = l;
            --r;
        }
        else
        {
            ++l;
            --r;
        }
    }

    quicksort(left, m - 1);
    quicksort(m + 1, right);
}

extern
int main(int argc, char * argv[])
{
    if ( argc != 2 )
    {
        printf("usage: quick_seq size\n");
        return EXIT_FAILURE;
    }

    size_t      size;

    if ( sscanf(argv[1], "%zu", &size) != 1 )
    {
        printf("bad size\n");
        return EXIT_FAILURE;
    }

    unsigned    * array = calloc(size, sizeof(unsigned));

    if ( array == NULL )
    {
        printf("no memory\n");
        return EXIT_FAILURE;
    }

    for ( size_t i = 0u;   i < size ;   ++i )
    {
        *(array + i) = rand() % size;
    }

    if ( size <= 30 )
    {
        print_array(array, size);
    }

    quicksort(array, array + size - 1);

    if ( size <= 30 )
    {
        print_array(array, size);
    }

    free(array);

    return EXIT_SUCCESS;
}
