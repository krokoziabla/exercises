#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
    size_t              n;

    if ( sscanf(argv[1], "%zu", &n) != 1 )
    {
        return EXIT_FAILURE;
    }

    printf("Matrix size: %zu\n", n);

    pid_t               * worker_pids = calloc(n, sizeof(pid_t));           assert( worker_pids != NULL );
    int                 * worker_pipe_fds = calloc(n << 2, sizeof(int));    assert( worker_pipe_fds != NULL );

#define R_WORKER        0
#define W_WORKER        1
#define R_CONTROLLER    2
#define W_CONTROLLER    3

    double              ** a = malloc(n * sizeof(double *));    assert( a != NULL );
    double              ** b = malloc(n * sizeof(double *));    assert( b != NULL );
    double              ** c = malloc(n * sizeof(double *));    assert( c != NULL );

    for ( size_t i = 0u;   i < n;   ++i )
    {
        a[i] = malloc(n * sizeof(double));                      assert( a[i] != NULL );
        b[i] = malloc(n * sizeof(double));                      assert( b[i] != NULL );
        c[i] = malloc(n * sizeof(double));                      assert( c[i] != NULL );

        for ( size_t j = 0u;   j < n;   ++j )
        {
            a[i][j] = 1.0;
            b[i][j] = 1.0;
            c[i][j] = -1.0;
        }
    }

    for ( size_t i = 0;   i < n;   ++i )
    {
        if ( pipe(worker_pipe_fds + (i << 2) + R_WORKER) == -1 )
        {
            printf("Failed to create pipe %zu\n", i);
            return EXIT_FAILURE;
        }
        if ( pipe(worker_pipe_fds + (i << 2) + R_CONTROLLER) == -1 )
        {
            printf("Failed to create pipe %zu\n", i);
            return EXIT_FAILURE;
        }

        worker_pids[i] = fork();

        switch ( worker_pids[i] )
        {
        case -1:

            printf("Failed to spawn worker %zu\n", i);
            return EXIT_FAILURE;

            break;

        case 0: // child
        {
            int             * pipe_fds = worker_pipe_fds + (i << 2);
            double          * in_row = calloc(n, sizeof(double));       assert( in_row != NULL );
            double          * out_row = calloc(n, sizeof(double));      assert( out_row != NULL );
            double          ** matrix = calloc(n, sizeof(double *));    assert( matrix != NULL );

            for ( size_t j = 0u;   j < n;   ++j )
            {
                matrix[j] = calloc(n, sizeof(double));                  assert( matrix[j] != NULL );
            }

            size_t          total_bytes_read = 0;

            while ( total_bytes_read != n * sizeof(double) )
            {
                ssize_t     bytes_read = read(pipe_fds[R_WORKER], in_row + total_bytes_read, n * sizeof(double) - total_bytes_read);

                if ( bytes_read == -1 )
                {
                    printf("Failed to read from pype in worker %zu\n", i);
                    return EXIT_FAILURE;
                }

                assert( bytes_read != 0 );

                total_bytes_read += bytes_read;
            }

            for ( size_t j = 0u;   j < n;   ++j )
            {
                total_bytes_read = 0;

                while ( total_bytes_read != n * sizeof(double) )
                {
                    ssize_t     bytes_read = read(*(pipe_fds + R_WORKER), matrix[j] + total_bytes_read, n * sizeof(double) - total_bytes_read);

                    if ( bytes_read == -1 )
                    {
                        printf("Failed to read from pype in worker %zu\n", i);
                        return EXIT_FAILURE;
                    }

                    assert( bytes_read != 0 );

                    total_bytes_read += bytes_read;
                }
            }

            for ( size_t j = 0u;   j < n;   ++j )
            {
                out_row[j] = 0.0;

                for ( size_t k = 0u;   k < n;   ++k )
                {
                    out_row[j] += in_row[k] * matrix[k][j];
                }
            }

            if ( write(*(pipe_fds + W_CONTROLLER), out_row, n * sizeof(double)) != n * sizeof(double) )
            {
                printf("Failed to write data to pipe in worker %zu\n", i);
                return EXIT_FAILURE;
            }

            return EXIT_SUCCESS;

            break;
        }
        default:

            for ( size_t j = 0u;   j < n;   ++j )
            {
                if ( write(*(worker_pipe_fds + (i << 2) + W_WORKER), a[i], n * sizeof(double)) != n * sizeof(double) )
                {
                    puts("Failed to write data to pipe in controller");
                    return EXIT_FAILURE;
                }

                for ( size_t k = 0u;   k < n;   ++k )
                {
                    if ( write(*(worker_pipe_fds + (i << 2) + W_WORKER), b[k], n * sizeof(double)) != n * sizeof(double) )
                    {
                        puts("Failed to write data to pipe in controller");
                        return EXIT_FAILURE;
                    }
                }
            }

            break;
        }
    }

    for ( size_t i = 0;   i < n;   ++i )
    {
        size_t          total_bytes_read = 0;

        while ( total_bytes_read != n * sizeof(double) )
        {
            ssize_t     bytes_read = read(*(worker_pipe_fds + (i << 2) + R_WORKER), c[i] + bytes_read, n * sizeof(double) - total_bytes_read);
            if ( bytes_read == -1 )
            {
                puts("Failed to read from pipe in controller");
                return EXIT_FAILURE;
            }

            assert( bytes_read != 0 );

            total_bytes_read += bytes_read;
        }

        int             status;

        if ( waitpid(worker_pids[i], &status, 0) == -1 )
        {
            printf("Failed to wait worker %zu\n", i);
            return EXIT_FAILURE;
        }

        if ( ! WIFEXITED(status) )
        {
            printf("Worker %zu hasn't exited\n", i);
            return EXIT_FAILURE;
        }

        if ( WEXITSTATUS(status) != EXIT_SUCCESS )
        {
            printf("Worker %zu failed\n", i);
            return EXIT_FAILURE;
        }

        if ( n < 10 )
        {
            for ( size_t j = 0;   j < n;   ++j )
            {
                printf("%5.2f ", c[i][j]);
            }
            putchar('\n');
        }
    }

    puts("Successfully multiplied");

    return EXIT_SUCCESS;
}
