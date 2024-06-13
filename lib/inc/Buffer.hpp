#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <string>
#include <mutex>

/*
 * 버퍼 string을 관리하는 함수
 * mutex를 사용하여 상호배제 적용
 */
class Buffer
{
private:
    std::string _buf;
    std::mutex _b_mutex;

public:
    Buffer();
    ~Buffer();

public: /* Buffer Control */
    void clear(void) noexcept;
    void fill(std::string str) noexcept;
    void fill(const char *str) noexcept;
    std::string flush(void) noexcept;
    void rollback(std::string str, std::size_t pos) noexcept;
    bool empty(void) const noexcept;

public: /* operator overloading */
    Buffer &operator=(const Buffer &rhs);
};

#endif
