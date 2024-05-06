#include "Buffer.hpp"

Buffer::Buffer() : _buf(std::string("")) {}

Buffer::~Buffer() {}

std::string Buffer::getBuf(void) noexcept
{
    std::string temp(_buf);

    _buf.clear();
    return temp;
}

std::string Buffer::getLine(void) noexcept
{
    std::size_t loc = _buf.find('\n');
    std::string result;

    if (loc != std::string::npos) {
        result = _buf.substr(0, loc);
        _buf = _buf.substr(loc + 1);
    }
    return result;
}

void Buffer::addBuf(const std::string &newBuf) noexcept
{
    _buf += newBuf;
}

void Buffer::rollBackBuf(std::string &remain, ssize_t len) noexcept
{
    _buf = remain.substr(len);
}
