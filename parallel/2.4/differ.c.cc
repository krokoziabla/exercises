#include <cassert>
#include <cstdlib>

#include <condition_variable>
#include <iostream>
#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

using namespace std;

static mutex                line1_mtx,
                            line2_mtx;

static condition_variable   line1_full_cond,
                            line2_full_cond;

static bool                 line1_full = false,
                            line2_full = false;

static volatile bool        stop = false;

static
void readline(fstream & f, unique_ptr<string> & l, mutex & l_mtx, condition_variable & l_full_cond, bool & l_full) noexcept
{
    unique_lock<mutex>      hang_on(l_mtx);

    while ( ! stop )
    {
        l_full_cond.wait(hang_on, [&l_full] () { return ! l_full; });

        if ( ! f.eof() )
        {
            getline(f, *l);
        }
        else
        {
            l.reset();
        }

        l_full = true;

        l_full_cond.notify_all();
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

    thread              th1(readline, ref(file1), ref(line1), ref(line1_mtx), ref(line1_full_cond), ref(line1_full)),
                        th2(readline, ref(file2), ref(line2), ref(line2_mtx), ref(line2_full_cond), ref(line2_full));

    while ( true )
    {
        unique_lock<mutex>      hang_on1(line1_mtx);

        line1_full_cond.wait(hang_on1, [] () { return line1_full; });

        unique_lock<mutex>      hang_on2(line2_mtx);

        line2_full_cond.wait(hang_on2, [] () { return line2_full; });

        if ( stop )
        {
            break;
        }
        else if ( line1 && line2 )
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
            stop = true;
        }

        line1_full = false;
        line2_full = false;

        line1_full_cond.notify_all();
        line2_full_cond.notify_all();

        ++counter;
    }

    th1.join();
    th2.join();

    return EXIT_SUCCESS;
}
