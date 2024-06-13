#include "Server.hpp"

Server::Server() : _mode(0) {}

Server::~Server() {}

void Server::init(void)
{
    int dummy = 1;

    Socket::init(); // 공통 초기화
    _addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 주소와 포트 재사용 옵션 추가
    // 서버 종료후 바로 재실행할 경우 기다려야하는 현상 방지
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &dummy, sizeof(dummy)) == -1)
        throw std::runtime_error("setsockopt() failed.");
    if (bind(_fd, reinterpret_cast<sockaddr *>(&_addr), sizeof(_addr)) == -1)
        throw std::runtime_error("bind() failed.");
    if (listen(_fd, BACKLOG) == -1)
        throw std::runtime_error("listen() failed.");
}

/*
 * 멀티 스레드 호출 함수
 * 클라이언트 연결 요청, 수신, 발신 함수를 각각의 스레드로 동작하게 함
 */
void Server::run(void)
{
    std::thread acceptThread(&Server::acceptLoop, std::ref(*this));
    std::thread recvThread(&Server::recvLoop, std::ref(*this));
    std::thread sendThread(&Server::sendLoop, std::ref(*this));

    acceptThread.join();
    recvThread.join();
    sendThread.join();
}

/*
 * 클라이언트 연결 수락 함수
 */
int Server::acceptClient(void)
{
    int client_fd;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    errno = 0;
    // 클라이언트 연결 수락
    client_fd = accept(_fd, reinterpret_cast<sockaddr *>(&addr), &len);
    if (client_fd == -1 && (errno != EAGAIN && errno != EWOULDBLOCK))
        throw std::runtime_error("accept() failed.");
    else if (client_fd == -1) // 재시도해야 할 경우 error throw 없이 종료
        return -1;
    // 논블록 옵션 설정
    if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1) {
        close(client_fd);
        throw std::runtime_error("fcntl() failed.");
    }
    /* 클라이언트 목록에 추가 */
    /* 연결 성공 메세지 버퍼에 추가(welcome) */
    addClient(client_fd);
    try
    {
        Socket &client = getClient(client_fd);
        client.addToWrite("WELCOME " + std::to_string(client_fd));
    }
    catch(const std::exception& e)
    {
        std::cerr << "acceptClient(): "<< e.what() << '\n';
        throw; // 현재 에러를 상위 try-catch문으로 전달
    }
    return client_fd;
}

void Server::addClient(int fd)
{
    std::lock_guard<std::mutex> lock(_s_mutex);
    _clients.insertNode(Socket(fd));
}

Socket &Server::getClient(int fd)
{
    std::lock_guard<std::mutex> lock(_s_mutex);
    MyTreeNode<Socket> *node = _clients.findNode(fd);
    if (node == nullptr)
        std::runtime_error("Client not find");
    return node->_data;
}

void Server::delClient(Socket &client)
{
    std::lock_guard<std::mutex> lock(_s_mutex);
    try
    {
        int fd = client.getFD();
        _clients.deleteNode(client);
        close(fd);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error closing fd " << client.getFD() << ": " << e.what() << '\n';
    }
}

/*
 * 멀티 스레드용 accept 루프 함수
 */
void Server::acceptLoop(void)
{
    std::cout << "accpet threaad on\n";
    while (true) {
        // lock 해제 후 쓰레드 양보를 위해 임의의 블록 스코프 생성
        {
            std::lock_guard<std::mutex> lock(_loop);
            if (_mode == 0) {
                try
                {
                    int fd = acceptClient();
                    if (fd != -1) {
                        std::cout << "accept success fd: " << fd << '\n';
                        Socket &client = getClient(fd);
                        // 연결됐음을 알리는 WELCOME MESSAGE 전송
                        client.addToWrite(std::string("WELCOME " + std::to_string(fd)));
                    }
                }
                catch (const std::exception &e)
                {
                    std::cerr << "accept Loop(): " << e.what() << '\n';
                }
                // 다른 쓰레드를 위해 모드 변경 후 쓰레드 양보 호출
                _mode = 1;
            }
        }
        std::this_thread::yield();
    }
}

/*
 * 멀티 스레드용 recv 루프 함수
 */
void Server::recvLoop(void)
{
    std::cout << "recv threaad on\n";
    while (true) {
        // lock 해제 후 쓰레드 양보를 위해 임의의 블록 스코프 생성
        {
            std::lock_guard<std::mutex> lock(_loop);
            if (_mode == 1) {
                for (MyTree<Socket>::Iterator it = _clients.begin(); it.hasNext();) {
                    Socket &now = it.next();
                    try
                    {
                        now.recvMsg();
                        std::cout << now.getFromRead();
                    }
                    catch (const std::exception &e)
                    {
                        std::cerr << "recv Loop(): " << e.what() << '\n';
                        delClient(now);
                        break ; // 데이터가 변경되면 반복자가 무효화되므로 반복문 탈출
                    }
                }
                // 다른 쓰레드를 위해 모드 변경 후 쓰레드 양보 호출
                _mode = 2;
            }
        }
        std::this_thread::yield();
    }
}

/*
 * 멀티 스레드용 send 루프 함수
 */
void Server::sendLoop(void)
{
    std::cout << "send threaad on\n";
    while (true) {
        // lock 해제 후 쓰레드 양보를 위해 임의의 블록 스코프 생성
        {
            std::lock_guard<std::mutex> lock(_loop);
            if (_mode == 2) {
                for (MyTree<Socket>::Iterator it = _clients.begin(); it.hasNext();) {
                    Socket &now = it.next();
                    try
                    {
                        now.sendMsg();
                    }
                    catch (const std::exception &e)
                    {
                        std::cerr << "send Loop(): " << e.what() << '\n';
                        delClient(now);
                        break ; // 데이터가 변경되면 반복자가 무효화되므로 반복문 탈출
                    }
                }
                // 다른 쓰레드를 위해 모드 변경 후 쓰레드 양보 호출
                _mode = 0;
            }
        }
        std::this_thread::yield();
    }
}
