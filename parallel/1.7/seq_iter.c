#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
    double      x0, x1;
    unsigned    n;

    if ( argc != 4 )
    {
        printf("Usage: seq_iter x0 x1 n\n");
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

    double      fleft = sin(x0) * exp(x0),
                fright,
                area = 0.0,
                width = (x1 - x0) / n;

    for ( unsigned i = 1;   i <= n;   ++i )
    {
        double x = x0 + width * i;

        fright = sin(x) * exp(x);
        area += (fleft + fright) * width / 2.0;
        fleft = fright;
    }

    printf("Area: %.10f\n", area);

    return EXIT_SUCCESS;
}
