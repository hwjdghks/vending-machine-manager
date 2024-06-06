#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <string>
#include <mutex>

class Buffer
{
private:
    std::string _buf;
    std::mutex _mutex;

public:
    Buffer();
    ~Buffer();

public:
    void clear(void) noexcept;
    void fill(std::string str) noexcept;
    void fill(const char *str) noexcept;
    std::string flush(void) noexcept;
    void rollback(std::string str, std::size_t pos) noexcept;
    bool empty(void) const noexcept;

public:
    Buffer &operator=(const Buffer &rhs);
};

#endif
