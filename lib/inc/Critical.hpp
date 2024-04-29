#ifndef CRITICAL_HPP
#define CRITICAL_HPP

#include "utils.hpp"
#include <mutex>

template<typename F, typename... A>
class Critical
{
private:
    std::mutex _mutex;

public:
    Critical();
    ~Critical();

public:
    void lock(std::mutex &mtx);
    void *execCriticalSection(F fp, A&&... args);
};


#endif
