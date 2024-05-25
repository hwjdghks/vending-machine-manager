#include "Client.hpp"

Client::Client() : _fd(-1) {}

Client::~Client()
{
    if (_fd != -1)
        close(_fd);
}

/*
 * 소켓 공통 초기화
 */
void Client::init(void)
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
void Client::serverInit(void)
{
    int dummy = 1;

    init(); // 공통 초기화
    _addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &dummy, sizeof(dummy)) == -1) // 포트 재사용 옵션 설정
        throw std::runtime_error("setsockopt() failed.");
    if (bind(_fd, reinterpret_cast<sockaddr *>(&_addr), sizeof(_addr)) == -1)
        throw std::runtime_error("bind() failed.");
    if (listen(_fd, BACKLOG) == -1)
        throw std::runtime_error("listen() failed.");
}

/*
 * 클라이언트 소켓 초기화
 */
void Client::clientInit(void)
{
    init();
    if (inet_pton(AF_INET, CONNECTION_IP, &_addr.sin_addr.s_addr) != 1)
        throw std::runtime_error("inet_pton() failed.");
}

void Client::tryConnect(void)
{
    if (connect(_fd, reinterpret_cast<sockaddr *>(&_addr), sizeof(_addr)) == -1)
        throw std::runtime_error("connect() failed.");
    run();
}

/*
 * 서버 구동
 */
void Client::run(void)
{
    std::thread recvThread(&Client::recvLoop, std::ref(*this));
    std::thread sendThread(&Client::sendLoop, std::ref(*this));

    recvThread.join();
    sendThread.join();
}

void Client::recvLoop(Client &client)
{
    while (true) { /* Need Edit */ /* 프로그램 종료 전까지 실행 */
        if (client.getFD() != -1) { // 소켓이 연결된 경우에만 실행
            try
            {
                client.recvMsg();
            }
            catch (const std::exception& e) // 에러 발생시 연결 종료
            {
                std::cerr << e.what() << '\n';
                client.closeFD();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void Client::sendLoop(Client &client)
{
    while (true) { /* Need Edit */ /* 프로그램 종료 전까지 실행 */
        if (client.getFD() != -1) { // 소켓이 연결된 경우에만 실행
            try
            {
                client.sendMsg();
            }
            catch (const std::exception& e) // 에러 발생시 연결 종료
            {
                std::cerr << e.what() << '\n';
                client.closeFD();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

/*
 * 메세지 수신
 */
int Client::recvMsg(void)
{
    std::lock_guard<std::mutex> lock(_mutex);
    char buf[BUFSIZE];

    std::memset(buf, 0, BUFSIZE);
    errno = 0;
    ssize_t len = recv(_fd, buf, BUFSIZE - 1, 0);
    if (len == -1 && (errno != EAGAIN && errno != EWOULDBLOCK)) // 재시도할 상황이 아닌 경우 에러 발생
        throw std::runtime_error("fd " + std::to_string(_fd) + ": recv() failed : " + std::strerror(errno));
    readBuf.fill(buf);
}

/*
 * 메세지 발송
 */
int Client::sendMsg(void)
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::string buf = writeBuf.flush();
    errno = 0;
    ssize_t len = send(_fd, buf.c_str(), buf.size(), 0);
    if (len == -1 && (errno != EAGAIN && errno != EWOULDBLOCK)) // 재시도할 상황이 아닌 경우 에러 발생
        throw std::runtime_error("fd " + std::to_string(_fd) + ": send() failed : " + std::strerror(errno));
    if (static_cast<std::size_t>(len) < buf.size()) // 일부분만 전송됐을 경우
        writeBuf.rollback(buf, len);
}

int Client::getFD(void)
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _fd;
}

void Client::setFD(int fd)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _fd = fd;
}

/*
 * 소켓 연결 종료
 */
void Client::closeFD(void)
{
    std::lock_guard<std::mutex> lock(_mutex);
    close(_fd);
    _fd = -1;
}

void Client::addToBuf(const std::string &str)
{
    readBuf.fill(str);
}

void Client::addToBuf(const char *str)
{
    readBuf.fill(str);
}

void Client::acceptLoop(Client &client)
{
    socklen_t len = sizeof(client._addr);
    errno = 0;
    int client_fd = accept(client._fd, reinterpret_cast<sockaddr *>(&client._addr), &len);
    if (client_fd == -1 && (errno != EAGAIN && errno != EWOULDBLOCK))
        throw std::runtime_error("accept() failed.");
}