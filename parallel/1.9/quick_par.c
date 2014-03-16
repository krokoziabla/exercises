#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>

static sem_t            thread_count;
static pthread_mutex_t  rand_mutex;

struct quicksort_arg
{
    int     * left;
    int     * right;
};




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
void quicksort(int * left, int * right);




static
void * worker(void * arg)
{
    struct quicksort_arg        * quicksort_arg = (struct quicksort_arg *)arg;

    quicksort(quicksort_arg->left, quicksort_arg->right);

    return NULL;
}




static
void quicksort(int * left, int * right)
{
    if ( right - left < 1 )
    {
        return;
    }
    // else right - left >= 2

    pthread_mutex_lock(&rand_mutex);

    int         * m = left + rand() % (right - left + 1),
                * l = left,
                * r = right;

    pthread_mutex_unlock(&rand_mutex);

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

    if ( sem_trywait(&thread_count) != 0 )
    {
        quicksort(left, m - 1);
        quicksort(m + 1, right);
    }
    else
    {
        pthread_t       left_worker,
                        right_worker;

        struct quicksort_arg        left_arg = { .left = left, .right = m - 1 },
                                    right_arg = { .left = m + 1, .right = right };

        pthread_create(&left_worker, NULL, worker, &left_arg);
        pthread_create(&right_worker, NULL, worker, &right_arg);

        pthread_join(left_worker, NULL);
        pthread_join(right_worker, NULL);

        sem_post(&thread_count);
    }
}

extern
int main(int argc, char * argv[])
{
    if ( argc != 3 )
    {
        printf("usage: quick_seq size\n");
        return EXIT_FAILURE;
    }

    size_t      size;
    unsigned    tc;

    if ( sscanf(argv[1], "%u", &tc) != 1 )
    {
        printf("bad thread number\n");
        return EXIT_FAILURE;
    }

    if ( sscanf(argv[2], "%zu", &size) != 1 )
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

    pthread_mutex_init(&rand_mutex, NULL);
    sem_init(&thread_count, 0, tc);

    quicksort(array, array + size - 1);

    if ( size <= 30 )
    {
        print_array(array, size);
    }

    free(array);

    return EXIT_SUCCESS;
}
