#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static double epsilon;

static
double quad(double left, double right, double fleft, double fright, double lrarea)
{
    double  mid = (left + right) / 2.0,
            fmid = sin(mid) * exp(mid),
            larea = (fleft + fmid) * (mid - left) / 2.0,
            rarea = (fmid + fright) * (right - mid) / 2.0;

    if ( fabs((larea + rarea) - lrarea) > epsilon )
    {
        larea = quad(left, mid, fleft, fmid, larea);
        rarea = quad(mid, right, fmid, fright, rarea);
    }

    return larea + rarea;
}

int main(int argc, char * argv[])
{
    double      x0, x1;

    if ( argc != 4 )
    {
        printf("Usage: seq_iter x0 x1 epsilon\n");
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

    if ( sscanf(argv[3], "%la", &epsilon) != 1 )
    {
        printf("Bad n\n");
        return EXIT_FAILURE;
    }

    double      fleft = sin(x0) * exp(x0),
                fright = sin(x1) * exp(x1),
                area = (fleft + fright) * (x1 - x0) / 2.0;

    area = quad(x0, x1, fleft, fright, area);

    printf("Area: %.10f\n", area);

    return EXIT_SUCCESS;
}
