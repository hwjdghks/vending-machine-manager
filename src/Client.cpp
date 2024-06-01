#include "Client.hpp"

Client::Client() : _isConnect(false) {}

Client::~Client() {}

/*
 * 클라이언트용 소켓 초기화 함수
 * 미리 매크로로 지정된 IP로 초기화
 */
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
        errno = 0;
        while (connect(_fd, reinterpret_cast<sockaddr *>(&_addr), sizeof(_addr)) == -1) {
            if (errno == EINPROGRESS)
                continue;
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

void Client::closeConnect(void)
{
    changeState();
    closeFD();
}

/*
 * recv와 send를 분리하여 멀티 쓰레드로 서버와 통신
 */
void Client::run(void)
{
    std::thread recvThread(&Client::recvLoop, std::ref(*this));
    std::thread sendThread(&Client::sendLoop, std::ref(*this));

    // join을 쓰면 여기서 멈추기 때문에 detach 사용
    recvThread.detach();
    sendThread.detach();
}

/*
 * 연결 상태 플래그 반환
 */
bool Client::isConnected(void)
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _isConnect;
}

/*
 * 연결 상태 변경
 */
void Client::changeState(void)
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_isConnect == false)
        _isConnect = true;
    else
        _isConnect = false;
}

/*
 * 멀티 쓰레드용 recv loop 함수
 * 연결중인 동안 루프를 돌면서 주기적으로 소켓에서 데이터를 읽어옴
 */
void Client::recvLoop(void)
{
    DebugLog::AddLog("recv thread on");
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
        std::this_thread::yield();
    }
}

/*
 * 멀티 쓰레드용 send loop 함수
 * 연결중인 동안 루프를 돌면서 주기적으로 소켓에 데이터를 보냄
 */
void Client::sendLoop(void)
{
    DebugLog::AddLog("send thread on");
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
        std::this_thread::yield();
    }
}