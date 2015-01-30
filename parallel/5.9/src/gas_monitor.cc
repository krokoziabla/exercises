#include "gas_monitor.hh"

using namespace std;

void gas_monitor::h_ready(id_t id) noexcept
{
    unique_lock<mutex>      guard(m);

    o.notify_all();
    l.push_back(id);
    h[id].wait(guard);
}

void gas_monitor::o_ready() noexcept
{
    unique_lock<mutex>      guard(m);

    list<id_t>              d;

    while ( d.empty() )
    {
        while ( l.empty() )
        {
            o.wait(guard);
        }

        // I could pick it randomly but
        // for symplicity pick the very first.

        d.splice(d.end(), l, l.begin());
    }

    // make water

    for ( auto & id : d )
    {
        h[id].notify_one();
    }
}
