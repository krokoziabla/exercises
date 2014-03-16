#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern
int main(int argc, char * argv[])
{
    int     fds[2];

    if ( pipe(fds) == -1 )
    {
        puts("Faled to create a pipe\n");
        return EXIT_FAILURE;
    }

    while ( true )
    {
        char message[10];

        snprintf(message, sizeof message, "hello%i", rand() % 10);

        puts(message);

        write(fds[1], message, strlen(message));
    }

    return EXIT_SUCCESS;
}
