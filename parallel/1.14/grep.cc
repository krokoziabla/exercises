#include <cassert>
#include <cstdlib>

#include <fstream>
#include <iostream>
#include <string>
#include <thread>

using namespace std;

static
void grep(istream & s, const string & pattern) noexcept
{
    string      line;

    while ( ! s.eof() )
    {
        getline(s, line);

        if ( line.find(pattern) != string::npos )
        {
            cout << line << endl;
        }
    }
}

extern
int main(int argc, char * argv[]) noexcept
{
    assert( argc == 4 );

    string      pattern(argv[1]);
    fstream     file1(argv[2], ios_base::in),
                file2(argv[3], ios_base::in);

    assert( file1.good() && file2.good() );

    thread      th1(grep, ref(file1), ref(pattern)),
                th2(grep, ref(file2), ref(pattern));

    th1.join();
    th2.join();

    return EXIT_SUCCESS;
}
