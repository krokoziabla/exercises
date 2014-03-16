#include <linux/limits.h>
#include <sys/types.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern
int main(int argc, char * argv[])
{
    int     parent2child[2],
            child2parent[2];

    if ( pipe(parent2child) == -1 || pipe(child2parent) == - 1)
    {
        puts("Faled to create a pipe");
        return EXIT_FAILURE;
    }


    pid_t   pid = fork();

    switch ( pid )
    {
    case -1:

        puts("Failed to fork a child");
        return EXIT_FAILURE;

        break;



    case 0:     // child

        puts("Listening to parent...");

        while ( true )
        {
            char        message[PIPE_BUF];
            ssize_t     bytes_read = read(parent2child[0], message, sizeof message);

            printf("Received from parent: *.*s\n", bytes_read, message);

            write(child2parent[1], message, bytes_read);
        }

        break;



    default:

        puts("Listening to child...");

        while ( true )
        {
            char        message[PIPE_BUF];
            ssize_t     bytes_read = read(child2parent[0], message, sizeof message);

            printf("Received from child: *.*s\n", bytes_read, message);

            write(parent2child[1], message, bytes_read);
        }
    }

    return EXIT_SUCCESS;
}
