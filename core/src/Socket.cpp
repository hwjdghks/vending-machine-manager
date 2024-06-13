#include "Socket.hpp"

Socket::Socket() : _fd(-1) {}

Socket::Socket(int fd) : _fd(fd) {}

Socket::Socket(const Socket &rhs)
{
    *this = rhs;
}

Socket::~Socket() {}

/*
 * 소켓 공통 초기화
 */
void Socket::init(void)
{
    _fd = socket(AF_INET, SOCK_STREAM, 0); // 소켓 생성
    if (_fd == -1)
        throw std::runtime_error("socket() failed.");
    if (fcntl(_fd, F_SETFL, O_NONBLOCK) == -1) // 논블럭 옵션 설정
        throw std::runtime_error("fcntl() failed.");
    std::memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(PORT);
}

/*
 * 메세지 수신
 */
void Socket::recvMsg(void)
{
    char buf[BUFSIZE];
    std::memset(buf, 0, BUFSIZE);

    int target = getFD();
    if (target == -1)
        return ;

    errno = 0;
    ssize_t len = recv(target, buf, BUFSIZE - 1, 0);
    if (len == -1 && (errno != EAGAIN && errno != EWOULDBLOCK)) // 재시도할 상황이 아닌 경우 에러 발생
        throw std::runtime_error("fd " + std::to_string(target) + ": recv() failed : " + std::strerror(errno) + " errno:" + std::to_string(errno));
    if (len == 0)
        throw std::runtime_error("fd " + std::to_string(target) + ": connection closed by peer");
    if (len > 0) {
        readBuf.fill(buf);
    }
}

/*
 * 메세지 발송
 */
void Socket::sendMsg(void)
{
    if (writeBuf.empty())
        return ;

    int target = getFD();
    if (target == -1)
        return ;

    std::string buf = writeBuf.flush();
    errno = 0;
    ssize_t len = send(target, buf.c_str(), buf.size(), 0);
    if (len == -1 && (errno != EAGAIN && errno != EWOULDBLOCK)) // 재시도할 상황이 아닌 경우 에러 발생
        throw std::runtime_error("fd " + std::to_string(target) + ": send() failed : " + std::strerror(errno) + " errno:" + std::to_string(errno));
    if (static_cast<std::size_t>(len) < buf.size()) // 일부분만 전송됐을 경우
        writeBuf.rollback(buf, len);
}

int Socket::getFD(void)
{
    std::lock_guard<std::mutex> lock(_so_mutex);
    return _fd;
}

void Socket::setFD(int fd)
{
    std::lock_guard<std::mutex> lock(_so_mutex);
    _fd = fd;
}

/*
 * 소켓 연결 종료
 */
void Socket::closeFD(void)
{
    std::lock_guard<std::mutex> lock(_so_mutex);
    if (_fd != -1) {
        std::cout << "Closing _fd: " << _fd << std::endl;
        close(_fd);
        _fd = -1;
    }
}

void Socket::addToWrite(const std::string &str)
{
    writeBuf.fill(str);
}

void Socket::addToWrite(const char *str)
{
    writeBuf.fill(str);
}

void Socket::addToRead(const std::string &str)
{
    readBuf.fill(str);
}

void Socket::addToRead(const char *str)
{
    readBuf.fill(str);
}

std::string Socket::getFromWrite(void)
{
    return writeBuf.flush();
}

std::string Socket::getFromRead(void)
{
    return readBuf.flush();
}

bool Socket::operator>(const Socket &rhs) const
{
    return this->_fd > rhs._fd;
}

bool Socket::operator<(const Socket &rhs) const
{
    return this->_fd < rhs._fd;
}

bool Socket::operator==(const Socket &rhs) const
{
    return this->_fd == rhs._fd;
}

Socket &Socket::operator=(const Socket &rhs)
{
    if (this == &rhs)
        return *this;
    this->_fd = rhs._fd;
    this->_id = rhs._id;
    this->_addr = rhs._addr;
    this->readBuf = rhs.readBuf;
    this->writeBuf = rhs.writeBuf;
    return *this;
}
