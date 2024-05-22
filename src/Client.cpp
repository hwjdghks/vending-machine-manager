#include "Client.hpp"

Client::Client() : _fd(-1) {}

Client::~Client()
{
    if (_fd != -1)
        close(_fd);
}

void Client::connect(void)
{
    try
    {
        int newFd;
        newFd = socket(AF_INET, SOCK_STREAM, 0);
        if (newFd == Enum<Try>::toInt(Try::FAILURE))
            throw std::runtime_error("Create socket failed"); /* Need Edit */
        int flag = 1;
	    if (setsockopt(newFd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == Enum<Try>::toInt(Try::FAILURE))
            throw std::runtime_error("Set socket option failed"); /* Need Edit */
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(9999); // 서버 포트
        inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
        // 논블록 소켓 설정
        if (fcntl(newFd, F_SETFL, O_NONBLOCK) == Enum<Try>::toInt(Try::FAILURE))
            throw std::runtime_error("Set nonblock socket failed"); /* Need Edit */
        // 서버에 연결
        if (::connect(newFd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == Enum<Try>::toInt(Try::FAILURE))
            throw std::runtime_error("Connection failed"); /* Need Edit */
        _mtxEtc.execCriticalSection(&Client::_setFd, this, newFd);
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << '\n';
        int fd = _mtxEtc.execCriticalSection(&Client::_getFd, this);
        if (fd != -1) {
            close(fd);
            _mtxEtc.execCriticalSection(&Client::_setFd, this, -1);
        }
        /* Print error and set close windows button */
    }
}

void Client::recvMsg(void)
{
    while (true) {
        try
        {
            _mtxRecv.execCriticalSection(&Client::_recv, this);
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

void Client::sendMsg(void)
{
    while (true) {
        try
        {
            _mtxSend.execCriticalSection(&Client::_send, this);
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

void Client::_recv(void)
{
    char buf[BUFSIZE];

    std::memset(buf, 0, BUFSIZE);
    int fd = _mtxEtc.execCriticalSection(&Client::_getFd, this);
    if (fd == -1)
        throw std::runtime_error("No have fd!"); /* Need Edit */
    ssize_t size = recv(fd, buf, BUFSIZE - 1, 0);
    if (size == -1)
        throw std::runtime_error("recv() failed"); /* Need Edit */
    buf[size] = '\0';
    _readBuf.addBuf(std::string(buf));
}

void Client::_send(void)
{
    std::string buf = _writeBuf.getBuf();
    try
    {
        int fd = _mtxEtc.execCriticalSection(&Client::_getFd, this);
        if (fd == -1)
            throw std::runtime_error("No have fd!"); /* Need Edit */
        ssize_t len = send(fd, buf.c_str(), buf.size(), 0);
        if (len == -1)
            throw std::runtime_error("send() failed"); /* Need Edit */
        if (static_cast<std::size_t>(len) < buf.size())
            _writeBuf.rollBackBuf(buf, len);
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
        _writeBuf.rollBackBuf(buf, 0);
    }
}

int Client::_getFd(void)
{
    return _fd;
}

void Client::_setFd(int fd)
{
    _fd = fd;
}