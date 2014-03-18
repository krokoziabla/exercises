#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

extern
int main(int argc, char * argv[])
{
    size_t          n, P, index, save_index;
    int             ret;

    int             in_fd,
                    out_fd;

    assert( argc == 5 );

    ret = sscanf(argv[0], "%zu", &n);
    assert( ret == 1 );

    ret = sscanf(argv[1], "%zu", &P);
    assert( ret == 1 );

    ret = sscanf(argv[2], "%i", &in_fd);
    assert( ret == 1 );

    ret = sscanf(argv[3], "%i", &out_fd);
    assert( ret == 1 );

    ret = sscanf(argv[4], "%zu", &index);
    assert( ret == 1 );

    save_index = index;

    double          ** column = calloc(P, sizeof(double *)),
                    ** in_row = calloc(P, sizeof(double)),
                    ** out_row = calloc(P, sizeof(double));

    assert( column != NULL );
    assert( in_row != NULL );
    assert( out_row != NULL );

    for ( size_t i = 0u;   i < P;   ++i )
    {
        in_row[i] = calloc(n * P, sizeof(double));
        assert( in_row[i] != NULL );

        column[i] = calloc(n * P, sizeof(double));
        assert( column[i] != NULL );

        out_row[i] = calloc(n * P, sizeof(double));
        assert( out_row[i] != NULL );

        ssize_t         bytes_handled = read(STDIN_FILENO, in_row[i], n * P * sizeof(double));

        if ( bytes_handled == -1 )
        {
            int error = errno;
            return EXIT_FAILURE;
        }

        assert( bytes_handled == n * P * sizeof(double));

        bytes_handled = read(STDIN_FILENO, column[i], n * P * sizeof(double));
        assert( bytes_handled == n * P * sizeof(double));
    }

    while ( true )
    {
        for ( size_t p = 0u;   p < P;   ++p )
        {
            for ( size_t j = index;   j < index + P;   ++j )
            {
                out_row[p][j] = 0.0;

                for ( size_t k = 0u;   k < n * P;   ++k )
                {
                    out_row[p][j] += in_row[p][k] * column[j % P][k];
                }
            }
        }

        for ( size_t p = 0u;   p < P;   ++p )
        {
            ssize_t     bytes_handled = write(out_fd, column[p], n * P * sizeof(double));
            assert( bytes_handled == n * P * sizeof(double) );
        }

        index = (index + P) % (n * P);

        if ( index == save_index )
        {
            break;
        }

        for ( size_t p = 0u;   p < P;   ++p )
        {
            ssize_t     bytes_handled = read(in_fd, column[p], n * P * sizeof(double));
            assert( bytes_handled == n * P * sizeof(double) );
        }
    }

    for ( size_t p = 0u;   p < P;   ++p )
    {
        ssize_t         bytes_handled = write(STDOUT_FILENO, out_row[p], n * P * sizeof(double));
        assert( bytes_handled == n * P * sizeof(double) );
    }

    return EXIT_SUCCESS;
}

