#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

extern
int main(int argc, char * argv[])
{
    size_t          n, P;
    int             ret;

    assert( argc == 2 );

    ret = sscanf(argv[0], "%zu", &n);
    assert( ret == 1 );

    ret = sscanf(argv[1], "%zu", &P);
    assert( ret == 1 );

    double          ** matrix = calloc(n * P, sizeof(double *)),
                    ** in_row = calloc(P, sizeof(double)),
                    ** out_row = calloc(P, sizeof(double));

    assert( matrix != NULL );
    assert( in_row != NULL );
    assert( out_row != NULL );

    for ( size_t i = 0u;   i < P;   ++i )
    {
        in_row[i] = calloc(n * P, sizeof(double));
        assert( in_row[i] != NULL );

        ssize_t         bytes_handled = read(STDIN_FILENO, in_row[i], n * P * sizeof(double));
        assert( bytes_handled == n * P* sizeof(double));
    }

    for ( size_t i = 0u;   i < n * P;   ++i )
    {
        matrix[i] = calloc(n * P, sizeof(double));
        assert( matrix[i] != NULL );

        ssize_t         bytes_handled = read(STDIN_FILENO, matrix[i], n * P * sizeof(double));
        assert( bytes_handled == n * P * sizeof(double));
    }

    for ( size_t p = 0u;   p < P;   ++p )
    {
        out_row[p] = calloc(n * P, sizeof(double));
        assert( out_row[p] != NULL );

        for ( size_t i = 0u;   i < n * P;   ++i )
        {
            out_row[p][i] = 0.0;

            for ( size_t j = 0u;   j < n * P;   ++j )
            {
                out_row[p][i] += in_row[p][j] * matrix[j][i];
            }
        }

        ssize_t         bytes_handled = write(STDOUT_FILENO, out_row[p], n * P * sizeof(double));
        assert( bytes_handled == n * P * sizeof(double) );
    }

    return EXIT_SUCCESS;
}
