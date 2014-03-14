#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#define WORKER_NUM 4

static struct coordinates
{
    double  x0;
    double  x1;
    unsigned n;
}                       coordinates[WORKER_NUM];

void * worker(void * arg)
{
    struct coordinates  * coordinates = (struct coordinates *)arg;

    double              x0 = coordinates->x0,
                        x1 = coordinates->x1;

    unsigned            n = coordinates->n;

    double              fleft = sin(x0) * exp(x0),
                        fright,
                        width = (x1 - x0) / n;

    double              * area = malloc(sizeof(double));

    assert( area != NULL );

    *area = 0.0;

    for ( unsigned i = 1;   i <= n;   ++i )
    {
        double x = x0 + width * i;

        fright = sin(x) * exp(x);
        *area += (fleft + fright) * width / 2.0;
        fleft = fright;
    }

    return (void *)area;
}

int main(int argc, char * argv[])
{
    double      x0, x1;
    unsigned    n;

    if ( argc != 4 )
    {
        printf("Usage: par_iter x0 x1 n\n");
        return EXIT_FAILURE;
    }

    if ( sscanf(argv[1], "%la", &x0) != 1 )
    {
        printf("Bad x0\n");
        return EXIT_FAILURE;
    }

    if ( sscanf(argv[2], "%la", &x1) != 1 )
    {
        printf("Bad x1\n");
        return EXIT_FAILURE;
    }

    if ( sscanf(argv[3], "%u", &n) != 1 )
    {
        printf("Bad n\n");
        return EXIT_FAILURE;
    }

    pthread_t   workers[WORKER_NUM];

    for ( size_t i = 0u;   i < WORKER_NUM;   ++i )
    {
        coordinates[i].x0 = x0 + i * (x1 - x0) / WORKER_NUM;
        coordinates[i].x1 = x0 + (i + 1u)  * (x1 - x0) / WORKER_NUM;
        coordinates[i].n = n / WORKER_NUM;

        int     ret = pthread_create(&workers[i], NULL, worker, &coordinates[i]);
        assert( ret == 0u );
    }

    double      area = 0.0;

    for ( size_t i = 0u;   i < WORKER_NUM;   ++i )
    {
        double  * area_fraction;

        int     ret = pthread_join(workers[i], (void **)&area_fraction);
        assert( ret == 0u );

        area += *area_fraction;
    }

    printf("Area: %.10f\n", area);

    return EXIT_SUCCESS;
}
