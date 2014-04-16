#include <cassert>
#include <cstdlib>

#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

using namespace std;

static
string                  line;

static
mutex                   line_mtx,
                        out_mtx;
static
condition_variable      line_empty_cv,
                        line_full_cv;

static
bool                    done = false,
                        other_written = false,
                        line_empty = false;

static
void read() noexcept
{
    if ( cin.eof() )
    {
        line_full_cv.notify_all();
        done = true;
        return;
    }

    unique_lock<mutex>  lck(line_mtx);

    line_empty_cv.wait(lck, [] () { return line.empty(); });

    getline(cin, line);

    line_empty = false;

    line_full_cv.notify_all();
}

static
void write(ostream & s) noexcept
{
    unique_lock<mutex>  lck(line_mtx);

    line_full_cv.wait(lck, [] () { return ! line_empty || done; });

    lck.unlock();

    if ( done )
    {
        return;
    }

    s << line << endl;

    unique_lock<mutex>  hang_on(out_mtx);

    if ( other_written )
    {
        line.clear();
        other_written = false;
        line_empty = true;

        line_empty_cv.notify_all();
    }
    else
    {
        other_written = true;
    }
}

extern
int main(int argc, char * argv[]) noexcept
{
    assert( argc == 2 );

    string      line;
    fstream     file(argv[1], ios_base::out);

    while ( ! done )
    {
        thread      reader(read),
                    out_writer(write, ref(cout)),
                    file_writer(write, ref(file));

        reader.join();
        out_writer.join();
        file_writer.join();
    }

    return EXIT_SUCCESS;
}
