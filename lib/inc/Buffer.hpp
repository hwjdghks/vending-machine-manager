#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <string>
#include <sys/types.h> /* Need Edit */

class Buffer
{
private:
    std::string _buf;

public:
    Buffer();
    ~Buffer();

public:
    std::string getBuf(void) noexcept;
    std::string getLine(void) noexcept;
    void addBuf(const std::string &newBuf) noexcept;
    void rollBackBuf(std::string &remain, ssize_t len) noexcept;
};

#endif
