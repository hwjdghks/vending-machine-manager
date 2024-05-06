#include "Critical.hpp"

Critical::Critical()  {}

Critical::~Critical() {}

void Critical::lock(std::mutex &mtx)
{
    std::lock_guard<std::mutex> guard(mtx);
}
