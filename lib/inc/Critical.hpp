#ifndef CRITICAL_HPP
#define CRITICAL_HPP

#include "utils.hpp"
#include <mutex>
#include <functional>

class Critical
{
private:
    std::mutex _mutex;

public:
    Critical();
    ~Critical();

public:
    void lock(std::mutex &mtx);

    template<typename F, typename... A>
    auto execCriticalSection(F&& fp, A&&... args)
    {
        std::lock_guard<std::mutex> guard(_mutex);
        return std::invoke(std::forward<F>(fp), std::forward<A>(args)...);
    }
};


#endif
