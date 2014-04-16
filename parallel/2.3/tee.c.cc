#include <cassert>
#include <cstdlib>

#include <condition_variable>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

using namespace std;

static mutex                line_mtx,
                            out_mtx;
static condition_variable   line_empty_cv;
static condition_variable   line_full_cv;

static string               line;

static bool                 done = false,
                            other_written = false;

static
void read() noexcept
{
    while ( ! cin.eof() )
    {
        unique_lock<mutex>      lck(line_mtx);

        line_empty_cv.wait(lck, [] () { return line.empty(); });

        getline(cin, line);

        line_full_cv.notify_all();
    }

    done = true;
}

static
void write(ostream & s) noexcept
{
    while ( true )
    {
        unique_lock<mutex>      lck(line_mtx);

        line_full_cv.wait(lck, [] () { return ! line.empty() || done; });

        lck.unlock();

        if ( done )
        {
            break;
        }

        s << line << endl;

        unique_lock<mutex>      out_mtx;

        if ( other_written )
        {
            line.clear();

            line_empty_cv.notify_all();

            other_written = false;
        }
        else
        {
            other_written = true;
        }
    }
}

extern
int main(int argc, char * argv[]) noexcept
{
    assert( argc == 2 );

    fstream     file(argv[1], ios_base::out);

    thread      reader(read);
    thread      cout_writer(write, ref(cout));
    thread      file_writer(write, ref(file));

    reader.join();
    cout_writer.join();
    file_writer.join();

    return EXIT_SUCCESS;
}
