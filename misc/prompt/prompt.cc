#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <string>

using namespace std;

static
void speak() noexcept
{
    const string    prompt("hello> ");
    string          line;

    char            c;

    cout << prompt;
    cout.flush();

    while ( true )
    {
        c = cin.get();

        switch ( c )
        {
        case '\n':

            puts("newline");
            cout << c << prompt;

            line.clear();

            break;

        case '\b':

            puts("backspace");
            if ( ! line.empty() )
            {
                cout << c;
                line.pop_back();
            }

            break;

        default:

            cout << c;

            line.push_back(c);
        }
        cout.flush();
    }
}

extern
int main(int argc, char * argv[])
{
    speak();

    return EXIT_SUCCESS;
}
