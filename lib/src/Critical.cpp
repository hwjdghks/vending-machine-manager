#include "Critical.hpp"

Critical::Critical()  {}

Critical::~Critical() {}

void Critical::lock(std::mutex &mtx)
{
    std::lock_guard<std::mutex> guard(mtx);
}

template<typename F, typename... A>
void *Critical::execCriticalSection(F fp, A&&... args)
{
    std::lock_guard<std::mutex> guard(_mutex);
    return fp(std::forward<Args>(args)...);
}