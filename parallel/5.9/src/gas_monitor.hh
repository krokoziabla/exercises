#include <list>
#include <mutex>
#include <condition_variable>
#include <vector>

class gas_monitor
{
public:

    typedef
    std::vector<std::condition_variable>    H;
    typedef
    H::size_type                            id;

public:

    void h_ready(id_t) noexcept;
    void o_ready() noexcept;

private:

    std::mutex                              m;
    std::condition_variable                 o;
    H                                       h;
    std::list<id_t>                         l;
};
