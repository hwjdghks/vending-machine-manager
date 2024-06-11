#include "Server.hpp"

Server::Server() : _mode(0) {}

Server::~Server() {}

void Server::init(void)
{
    int dummy = 1;

    Socket::init(); // 공통 초기화
    _addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &dummy, sizeof(dummy)) == -1)
        throw std::runtime_error("setsockopt() failed.");
    if (bind(_fd, reinterpret_cast<sockaddr *>(&_addr), sizeof(_addr)) == -1)
        throw std::runtime_error("bind() failed.");
    if (listen(_fd, BACKLOG) == -1)
        throw std::runtime_error("listen() failed.");
}

void Server::run(void)
{
    std::thread acceptThread(&Server::acceptLoop, std::ref(*this));
    std::thread recvThread(&Server::recvLoop, std::ref(*this));
    std::thread sendThread(&Server::sendLoop, std::ref(*this));

    acceptThread.join();
    recvThread.join();
    sendThread.join();
}

int Server::acceptClient(void)
{
    int client_fd;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    errno = 0;
    client_fd = accept(_fd, reinterpret_cast<sockaddr *>(&addr), &len);
    if (client_fd == -1 && (errno != EAGAIN && errno != EWOULDBLOCK))
        throw std::runtime_error("accept() failed.");
    else if (client_fd == -1)
        return -1;
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
        throw;
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
    try
    {
        std::lock_guard<std::mutex> lock(_s_mutex);
        MyTreeNode<Socket> *node = _clients.findNode(fd);
        if (node == nullptr)
            std::runtime_error("Client not find");
        return node->_data;
    }
    catch(const std::system_error& e)
    {
        std::cerr << "getClient(): " << e.what() << '\n';
        throw;
    }
    catch(const std::exception& e)
    {
        std::cerr << "getClient(): " << e.what() << '\n';
        throw;
    }
}

void Server::delClient(Socket &client)
{
    std::lock_guard<std::mutex> lock(_s_mutex);
    try
    {
        client.closeFD();
        _clients.deleteNode(client);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error closing fd " << client.getFD() << ": " << e.what() << '\n';
    }
}

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
                    }
                }
                catch (const std::system_error &e)
                {
                    std::cerr << "system_error: accept Loop(): " << e.what() << '\n';
                }
                catch (const std::exception &e)
                {
                    std::cerr << "accept Loop(): " << e.what() << '\n';
                }
                _mode = 1;
            }
        }
        std::this_thread::yield();
    }
}

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
                _mode = 2;
            }
        }
        std::this_thread::yield();
    }
}

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
                _mode = 0;
            }
        }
        std::this_thread::yield();
    }
}
