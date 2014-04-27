#include <cassert>
#include <cstdlib>

#include <iostream>
#include <fstream>
#include <memory>
#include <string>

using namespace std;

extern
int main(int argc, char * argv[]) noexcept
{
    assert( argc == 3 );

    fstream             file1(argv[1], ios_base::in),
                        file2(argv[2], ios_base::in);

    unique_ptr<string>  line1(new string),
                        line2(new string);

    unsigned            counter = 0u;

    while ( true )
    {
        if ( ! file1.eof() )
        {
            getline(file1, *line1);
        }
        else
        {
            line1.reset();
        }
        if ( ! file2.eof() )
        {
            getline(file2, *line2);
        }
        else
        {
            line2.reset();
        }

        if ( line1 && line2 )
        {
            if ( *line1 != *line2 )
            {
                cout << counter << ": " << *line1 << endl;
                cout << counter << ": " << *line2 << endl;
            }
        }
        else if ( line1 )
        {
            cout << counter << ": " << *line1 << endl;
        }
        else if ( line2 )
        {
            cout << counter << ": " << *line2 << endl;
        }
        else
        {
            break;
        }

        ++counter;
    }

    return EXIT_SUCCESS;
}
