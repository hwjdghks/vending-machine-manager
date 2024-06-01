#include "Socket.hpp"

Socket::Socket() : _fd(-1) {}

Socket::~Socket()
{
    if (_fd != -1)
        close(_fd);
}

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
 * 서버 소켓 초기화
 */
// void Socket::serverInit(void)
// {
//     int dummy = 1;

//     init(); // 공통 초기화
//     _addr.sin_addr.s_addr = htonl(INADDR_ANY);
//     if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &dummy, sizeof(dummy)) == -1) // 포트 재사용 옵션 설정
//         throw std::runtime_error("setsockopt() failed.");
//     if (bind(_fd, reinterpret_cast<sockaddr *>(&_addr), sizeof(_addr)) == -1)
//         throw std::runtime_error("bind() failed.");
//     if (listen(_fd, BACKLOG) == -1)
//         throw std::runtime_error("listen() failed.");
// }

/*
 * 클라이언트 소켓 초기화
 */
// void Socket::clientInit(void)
// {
//     init();
//     if (inet_pton(AF_INET, CONNECTION_IP, &_addr.sin_addr.s_addr) != 1)
//         throw std::runtime_error("inet_pton() failed.");
// }

// void Socket::tryConnect(void)
// {
//     if (connect(_fd, reinterpret_cast<sockaddr *>(&_addr), sizeof(_addr)) == -1)
//         throw std::runtime_error("connect() failed.");
//     run();
// }

/*
 * 서버 구동
 */
// void Socket::run(void)
// {
//     std::thread acceptLoop(&Socket::acceptLoop, std::ref(*this));
//     std::thread recvThread(&Socket::recvLoop, std::ref(*this));
//     std::thread sendThread(&Socket::sendLoop, std::ref(*this));

//     acceptLoop.join();
//     recvThread.join();
//     sendThread.join();
// }

/*
 * 메세지 수신
 */
int Socket::recvMsg(void)
{
    char buf[BUFSIZE];

    std::memset(buf, 0, BUFSIZE);
    errno = 0;
    ssize_t len = recv(getFD(), buf, BUFSIZE - 1, 0);
    // perror(strerror(errno));
    if (len == -1 && (errno != EAGAIN && errno != EWOULDBLOCK)) // 재시도할 상황이 아닌 경우 에러 발생
        throw std::runtime_error("fd " + std::to_string(_fd) + ": recv() failed : " + std::strerror(errno));
    if (len > 0) {
        printf("recv: %s\n", buf);
        readBuf.fill(buf);
    }
}

/*
 * 메세지 발송
 */
int Socket::sendMsg(void)
{
    if (writeBuf.empty())
        return 1;

    std::string buf = writeBuf.flush();
    printf("send: %s\n", buf.c_str());
    errno = 0;
    ssize_t len = send(getFD(), buf.c_str(), buf.size(), 0);
    // perror(strerror(errno));
    if (len == -1 && (errno != EAGAIN && errno != EWOULDBLOCK)) // 재시도할 상황이 아닌 경우 에러 발생
        throw std::runtime_error("fd " + std::to_string(_fd) + ": send() failed : " + std::strerror(errno));
    if (static_cast<std::size_t>(len) < buf.size()) // 일부분만 전송됐을 경우
        writeBuf.rollback(buf, len);
}

int Socket::getFD(void)
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _fd;
}

void Socket::setFD(int fd)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _fd = fd;
}

/*
 * 소켓 연결 종료
 */
void Socket::closeFD(void)
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_fd != -1)
        close(_fd);
    _fd = -1;
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
