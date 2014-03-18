#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <unistd.h>



int main(int argc, char * argv[])
{
    size_t          n, P;
    int             ret;

    assert( argc == 3 );

    ret = sscanf(argv[1], "%zu", &n);
    assert( ret == 1 );

    ret = sscanf(argv[2], "%zu", &P);
    assert( ret == 1 );

    printf("Matrix size %zu\n", n * P);

    double          ** matrix = calloc(n * P, sizeof(double *));
    assert( matrix != NULL );

    int             * pfd = calloc(n << 2, sizeof(int)),
                    * pfd_ring = calloc(n << 1, sizeof(int));
    assert( pfd != NULL );
    assert( pfd_ring != NULL );

#define R_WORKER       0
#define W_WORKER       1
#define R_CONTROLLER   2
#define W_CONTROLLER   3

    for ( size_t i = 0u;   i < n * P;   ++i )
    {
        matrix[i] = calloc(n * P, sizeof(double));
        assert( matrix[i] != NULL );

        for ( size_t j = 0u;   j < n * P;   ++j )
        {
            matrix[i][j] = 1.0;
        }
    }

    for ( size_t i = 0u;   i < n;   ++i )
    {
        ret = pipe(pfd_ring + (i << 1u));
        assert( ret != -1 );
    }

    pid_t           * pid = calloc(n, sizeof(pid_t));
    assert( pid != NULL );

    for ( size_t i = 0u;   i < n;   ++i )
    {
        ret = pipe(pfd + (i << 2u) + R_WORKER);
        assert( ret != -1 );
        ret = pipe(pfd + (i << 2u) + R_CONTROLLER);
        assert( ret != -1 );

        for ( int * j = pfd + (i << 2);   j < pfd + ((i + 1u) << 2);   ++j )
        {
            ret = fcntl(*j, F_SETFD, FD_CLOEXEC);
            assert( ret != -1 );
        }

        pid[i] = fork();

        switch ( pid[i] )
        {
        case -1:

            printf("Failed to spawn worker %zu\n", i);
            return EXIT_FAILURE;

        case 0: // child
        {
            ret = dup2(pfd[(i << 2u) + R_WORKER], STDIN_FILENO);
            assert( ret != -1 );
            ret = dup2(pfd[(i << 2u) + W_CONTROLLER], STDOUT_FILENO);
            assert( ret != -1 );

            ret = close(pfd_ring[((i + 0u) % n << 1u) + 1u]); // close in write end
            assert( ret != -1 );

            ret = close(pfd_ring[((i + 1u) % n << 1u) + 0u]); // close out read end
            assert( ret != -1 );

            char    in_fd[33],
                    out_fd[33],
                    index[33];

            snprintf(in_fd, sizeof in_fd, "%i", pfd_ring[((i + 0u) % n << 1u) + 0u]);
            snprintf(out_fd, sizeof out_fd, "%i", pfd_ring[((i + 1u) % n << 1u) + 1u]);
            snprintf(index, sizeof index, "%zu", i * P);

            ret = execl("./ring_worker", argv[1], argv[2], in_fd, out_fd, index, NULL);
            assert( ret != -1 );

            break;
        }
        default: // parent
        {
            ret = close(pfd[(i << 2u) + R_WORKER]);
            assert( ret != -1 );
            ret = close(pfd[(i << 2u) + W_CONTROLLER]);
            assert( ret != -1 );

            for ( size_t j = 0u;   j < P;   ++j )
            {
                ssize_t     bytes_handled = write(pfd[(i << 2u) + W_WORKER], matrix[i + j], n * P * sizeof(double));
                assert( bytes_handled == n * P * sizeof(double) );

                // Here we pretend we are sending P columns;
                bytes_handled = write(pfd[(i << 2u) + W_WORKER], matrix[i + j], n * P * sizeof(double));
                assert( bytes_handled == n * P * sizeof(double) );
            }

            break;
        }
        }
    }

    for ( size_t i = 0u;   i < n;   ++i )
    {
        for ( size_t j = 0u;   j < P;   ++j )
        {
            ssize_t     bytex_handled = read(pfd[(i << 2u) + R_CONTROLLER], matrix[i + j], n * P * sizeof(double));
            assert( bytex_handled == n * P * sizeof(double) );
        }

        if ( n * P < 10 )
        {
            for ( size_t j = 0u;   j < n * P;   ++j )
            {
                printf("%3.1f\t", matrix[i][j]);
            }
            putchar('\n');
        }

        int status;

        ret = waitpid(pid[i], &status, 0);
        assert( ret != -1 );

        assert( WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS );
    }

    puts("Matrices multiplied successfully");

    return EXIT_SUCCESS;
}
