#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>

static double   epsilon;
static sem_t    launched_threads;

struct quad_args
{
    double left;
    double right;
    double fleft;
    double fright;
    double lrarea;
};

static
double quad(double left, double right, double fleft, double fright, double lrarea);

static
void * worker(void * arg)
{
    struct quad_args        * quad_args = (struct quad_args *)arg;

    quad_args->lrarea = quad(quad_args->left, quad_args->right, quad_args->fleft, quad_args->fright, quad_args->lrarea);

    return quad_args;
}

static
double quad(double left, double right, double fleft, double fright, double lrarea)
{
    double  mid = (left + right) / 2.0,
            fmid = sin(mid) * exp(mid),
            larea = (fleft + fmid) * (mid - left) / 2.0,
            rarea = (fmid + fright) * (right - mid) / 2.0;

    if ( fabs((larea + rarea) - lrarea) > epsilon )
    {
        if ( sem_trywait(&launched_threads) == -1 )
        {
            larea = quad(left, mid, fleft, fmid, larea);
            rarea = quad(mid, right, fmid, fright, rarea);
        }
        else
        {
            struct quad_args    left_quad_args = { .left = left, .right = mid, .fleft = fleft, .fright = fmid, .lrarea = larea },
                                right_quad_args = { .left = mid, .right = right, .fleft = fmid, .fright = fright, .lrarea = rarea };

            pthread_t           left_worker,
                                right_worker;

            pthread_create(&left_worker, NULL, worker, &left_quad_args);
            pthread_create(&right_worker, NULL, worker, &right_quad_args);

            pthread_join(left_worker, NULL);
            pthread_join(right_worker, NULL);

            larea = left_quad_args.lrarea;
            rarea = right_quad_args.lrarea;

            sem_post(&launched_threads);
        }
    }

    return larea + rarea;
}

int main(int argc, char * argv[])
{
    double      x0, x1;

    unsigned    thread_pairs;

    if ( argc != 5 )
    {
        printf("Usage: par_rec x0 x1 thread_num epsilon\n");
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

    if ( sscanf(argv[3], "%u", &thread_pairs) != 1 )
    {
        printf("Bad number of thread pairs\n");
        return EXIT_FAILURE;
    }

    if ( sscanf(argv[4], "%la", &epsilon) != 1 )
    {
        printf("Bad prescsion\n");
        return EXIT_FAILURE;
    }

    if ( sem_init(&launched_threads, 0, thread_pairs) != 0 )
    {
        printf("Failed to init a semaphore\n");
        return EXIT_FAILURE;
    }



    double      fleft = sin(x0) * exp(x0),
                fright = sin(x1) * exp(x1),
                area = (fleft + fright) * (x1 - x0) / 2.0;

    area = quad(x0, x1, fleft, fright, area);

    printf("Area: %.10f\n", area);

    return EXIT_SUCCESS;
}
