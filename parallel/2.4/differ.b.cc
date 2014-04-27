#include <cassert>
#include <cstdlib>

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <thread>

using namespace std;

static
void readline(fstream & f, unique_ptr<string> & l) noexcept
{
    if ( ! f.eof() )
    {
        getline(f, *l);
    }
    else
    {
        l.reset();
    }
}

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
        thread                  th1(readline, ref(file1), ref(line1)),
                                th2(readline, ref(file2), ref(line2));

        th1.join();
        th2.join();

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
