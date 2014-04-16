#include <cassert>
#include <cstdlib>

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

extern
int main(int argc, char * argv[]) noexcept
{
    assert( argc == 2 );

    string      line;
    fstream     file(argv[1], ios_base::out);

    while ( ! cin.eof() )
    {
        getline(cin, line);

        cout << line << endl;
        file << line << endl;
    }

    return EXIT_SUCCESS;
}
