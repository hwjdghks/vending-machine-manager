#include "Buffer.hpp"

Buffer::Buffer() : _buf(std::string("")) {}

Buffer::~Buffer() {}

void Buffer::clear(void) noexcept
{
    std::lock_guard<std::mutex> lock(_mutex);
    _buf.clear();
}

void Buffer::fill(std::string str) noexcept
{
    std::lock_guard<std::mutex> lock(_mutex);
    _buf += str;
}

void Buffer::fill(const char *str) noexcept
{
    std::lock_guard<std::mutex> lock(_mutex);
    _buf += str;
}

std::string Buffer::flush(void) noexcept
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::string ret = std::move(_buf);
    _buf.clear();
    return ret;
}

void Buffer::rollback(std::string str, std::size_t pos) noexcept
{
    std::lock_guard<std::mutex> lock(_mutex);
    _buf.insert(0, str.substr(pos));
}

bool Buffer::empty(void) const noexcept
{
    return _buf.empty();
}