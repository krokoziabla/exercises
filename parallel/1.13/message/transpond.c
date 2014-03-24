#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static
void print_matrix(int ** matrix, size_t n)
{
    if ( n <= 10u )
    {
        for ( size_t i = 0u;   i < n;   ++i )
        {
            for ( size_t j = 0u;   j < n;   ++j )
            {
                printf("%i\t", matrix[i][j]);
            }
            putchar('\n');
        }
    }
}

extern
int main(int argc, char * argv[])
{
    size_t          n,
                    P,
                    np;

    int             ret = -1;

    assert( argc == 3 );

    ret = sscanf(argv[1], "%zu", &n);
    assert( ret == 1 );

    ret = sscanf(argv[2], "%zu", &P);
    assert( ret == 1 );

    int             ** matrix = calloc(n, sizeof(int *));
    assert( matrix != NULL );

    for ( size_t i = 0u;   i < n;   ++i )
    {
        matrix[i] = calloc(n, sizeof(int));
        assert( matrix[i] != NULL );

        for ( size_t j = 0u;   j < n;   ++j )
        {
            matrix[i][j] = rand() % 30;
        }
    }

    print_matrix(matrix, n);

    pid_t           * pid = calloc(P + 1u, sizeof(pid_t));
    assert( pid != NULL );

    int             * pfd = calloc(P + 1u << 2, sizeof(int));
    assert( pid != NULL );

#define R_WORKER        0
#define W_WORKER        1
#define R_CONTROLLER    2
#define W_CONTROLLER    3

    np = ((n << 1u) - 1u) / P;

    size_t          current_worker = np;

    for ( size_t s = 0u;   s < (n << 1u) - 1u;   ++s )
    {
        if ( s % np == 0u )
        {
            current_worker = s / np + 1u;

            ret = pipe(pfd + (current_worker << 2u) + R_WORKER);
            assert( ret != -1 );

            ret = pipe(pfd + (current_worker << 2u) + R_CONTROLLER);
            assert( ret != -1 );

            ret = close(pfd[current_worker - np + W_WORKER]);
            // assert( ret != -1 || errno == EINVAL );

            pid[current_worker] = fork();
            assert( pid[current_worker] != -1 );

            if ( pid[current_worker] == 0 ) // child
            {
                ret = close(pfd[(current_worker << 2u) + W_WORKER]);
                assert( ret != -1 );
                ret = close(pfd[(current_worker << 2u) + R_CONTROLLER]);
                assert( ret != -1 );

                ret = dup2(pfd[(current_worker << 2u) + R_WORKER], STDIN_FILENO);
                assert( ret != -1 );

                ret = dup2(pfd[(current_worker << 2u) + W_CONTROLLER], STDOUT_FILENO);
                assert( ret != -1 );

                ret = execl("./worker", NULL);

                return EXIT_FAILURE;
            }

        }

        size_t          T = s < n ? s / 2 : (((n - 1u) << 1u) - s) / 2;

        ssize_t         bytes_handled = -1;

//        ssize_t         bytes_handled = write(pfd[(current_worker << 2u) + W_WORKER], &T, sizeof T);
//        assert( bytes_handled == sizeof T );

        for ( size_t t = 0u;   t <= T;   ++t )
        {
            size_t      i = s + t - s / 2,
                        j = s / 2 - t;

            bytes_handled = write(pfd[(current_worker << 2u) + W_WORKER], &matrix[i][j], sizeof matrix[i][j]);
            assert( bytes_handled == sizeof matrix[i][j] );

            bytes_handled = write(pfd[(current_worker << 2u) + W_WORKER], &matrix[j][i], sizeof matrix[j][i]);
            assert( bytes_handled == sizeof matrix[j][i] );
        }
    }
    ret = close(pfd[(current_worker << 2u) + W_WORKER]);
    assert( ret != -1 );

    for ( size_t s = 0u;   s < (n << 1u) - 1u;  ++s )
    {
        if ( s % np == 0u )
        {
            int         status;

            waitpid(pid[s / np], NULL, 0);
            current_worker = s / np + 1u;
        }

        size_t          T = s < n ? s / 2 : (((n - 1u) << 1u) - s) / 2;
        ssize_t         bytes_handled = 0;

        for ( size_t t = 0u;    t <= T;   ++t )
        {
            size_t      i = s + t - s / 2,
                        j = s / 2 - t;

            bytes_handled = read(pfd[(current_worker << 2u) + R_CONTROLLER], &matrix[i][j], sizeof matrix[i][j]);
            assert( bytes_handled == sizeof matrix[i][j] );

            bytes_handled = read(pfd[(current_worker << 2u) + R_CONTROLLER], &matrix[j][i], sizeof matrix[j][i]);
            assert( bytes_handled == sizeof matrix[j][i] );
        }
    }
    ret = close(pid[P]);
    assert( ret != -1 );

    putchar('\n');

    print_matrix(matrix, n);

    return EXIT_SUCCESS;
}
