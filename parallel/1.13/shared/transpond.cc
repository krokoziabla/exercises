#include <cassert>
#include <cstdlib>

#include <array>
#include <chrono>
#include <iostream>
#include <random>
#include <sstream>
#include <thread>

using namespace std;

typedef vector<vector<int>>         matrix_t;

static
void print_matrix(matrix_t matrix)
{
    if ( matrix.size() <= 10u )
    {
        for ( auto & row : matrix )
        {
            for ( auto & element : row )
            {
                cout << element << '\t';
            }
            cout << endl;
        }
    }
}

static
int worker(matrix_t & matrix, matrix_t::size_type start, matrix_t::size_type number, matrix_t::size_type size)
{
    for ( size_t i = start;   i < start + number;   ++i )
    {
        for ( size_t j = i + 1;   j < size;   ++j )
        {
            matrix[i][j] = matrix[i][j] + matrix[j][i];
            matrix[j][i] = matrix[i][j] - matrix[j][i];
            matrix[i][j] = matrix[i][j] - matrix[j][i];
        }
    }
}

extern
int main(int argc, char * argv[])
{
    size_t                  n = 0u,
                            P = 0u,
                            np;

    int                     ret = -1;

    istringstream           arg_stream;

    assert( argc == 3 );

    arg_stream.str(argv[1]);
    arg_stream >> n;
    assert( arg_stream.eof() );
    arg_stream.clear();

    arg_stream.str(argv[2]);
    arg_stream >> P;
    assert( arg_stream.eof() );
    arg_stream.clear();

    np = n / P;

    matrix_t                matrix(n);
    vector<thread>          threads;

    unsigned                seed = chrono::system_clock::now().time_since_epoch().count();
    minstd_rand0            generator(seed);

    for ( auto & row : matrix )
    {
        for ( auto i = 0u;   i < n;   ++i )
        {
            row.push_back(generator() % 30);
        }
    }

    print_matrix(matrix);

    for ( auto i = 0u;   i < P - 1u;   ++i )
    {
        threads.push_back(thread(worker, ref(matrix), i * np, np, n));
    }
    threads.push_back(thread(worker, ref(matrix), (P - 1u) * np, n - (P - 1u) * np, n));

    for ( auto & thread : threads )
    {
        thread.join();
    }

    putchar('\n');

    print_matrix(matrix);

    return EXIT_SUCCESS;
}
