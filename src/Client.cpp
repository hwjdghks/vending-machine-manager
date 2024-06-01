#include "Client.hpp"

Client::Client() : _isConnect(false) {}

Client::~Client() {}

void Client::init(void)
{
    try
    {
        Socket::init();
        if (inet_pton(AF_INET, CONNECTION_IP, &_addr.sin_addr) != 1)
            throw std::runtime_error("inet_pton() failed.");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        closeFD();
    }
}

void Client::tryConnect(void)
{
    try
    {
        init();
retry:
        errno = 0;
        if (connect(_fd, reinterpret_cast<sockaddr *>(&_addr), sizeof(_addr)) == -1) {
            if (errno != EINPROGRESS)
                goto retry;
            else {
                perror(strerror(errno));
                throw std::runtime_error("connect() failed.");
            }
        }
        changeState();
        run();
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
        closeFD();
    }
}

void Client::run(void)
{
    std::thread recvThread(&Client::recvLoop, std::ref(*this));
    std::thread sendThread(&Client::sendLoop, std::ref(*this));

    // join을 쓰면 여기서 멈추기 때문에 detach 사용
    recvThread.detach();
    sendThread.detach();
}

bool Client::isConnected(void)
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _isConnect;
}

void Client::changeState(void)
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_isConnect == false)
        _isConnect = true;
    else
        _isConnect = false;
}

void Client::recvLoop(void)
{
    while (isConnected()) {
        try
        {
            recvMsg();
        }
        catch(const std::runtime_error& e)
        {
            std::cerr << e.what() << '\n';
            changeState();
        }
    }
}

void Client::sendLoop(void)
{
    while (isConnected()) {
        try
        {
            sendMsg();
        }
        catch(const std::runtime_error& e)
        {
            std::cerr << e.what() << '\n';
            changeState();
        }
    }
}