#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include <unistd.h>

int main(int argc, char * argv[])
{
    while ( true )
    {
        int     ij,
                ji;

        ssize_t bytes_handled = read(STDIN_FILENO, &ij, sizeof ij);
        assert( bytes_handled != -1 );

        if ( bytes_handled == 0 )
        {
            break;
        }

        bytes_handled = read(STDIN_FILENO, &ji, sizeof ji);
        assert( bytes_handled == sizeof ji );

        bytes_handled = write(STDOUT_FILENO, &ji, sizeof ji);
        assert( bytes_handled == sizeof ji );

        bytes_handled = write(STDOUT_FILENO, &ij, sizeof ij);
        assert( bytes_handled == sizeof ij );
    }
    return EXIT_SUCCESS;
}
