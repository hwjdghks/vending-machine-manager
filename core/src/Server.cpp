#include "Server.hpp"

Server::Server() {}

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
    return client_fd;
}

void Server::addClient(int fd)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _clients.insertNode(Socket(fd));
}

Socket &Server::getClient(int fd)
{
    std::lock_guard<std::mutex> lock(_mutex);
    MyTreeNode<Socket> *node = _clients.findNode(fd);
    if (!node)
        std::runtime_error("Client not find");
    return node->_data;
}

void Server::delClient(int fd)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _clients.deleteNode(Socket(fd));
}

void Server::acceptLoop(void)
{
    std::cout << "accpet threaad on\n";
    while (true) {
        try
        {
            std::lock_guard<std::mutex> lock(_loop);
            int fd = acceptClient();
            if (fd != -1) {
                Socket &client = getClient(fd);
                client.addToWrite(std::string("WELCOME " + std::to_string(fd)));
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        std::this_thread::yield();
    }
}

void Server::recvLoop(void)
{
    std::cout << "recv threaad on\n";
    Socket *save;

    while (true) {
        try
        {
            std::lock_guard<std::mutex> lock(_loop);
            for (MyTree<Socket>::Iterator it = _clients.begin(); it.hasNext(); ) {
                Socket &now = it.next();
                save = &now;
                now.recvMsg();
                // parse(now.getFromRead());
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            delClient(save->getFD());
        }
        std::this_thread::yield();
        sleep(1);
    }
}

void Server::sendLoop(void)
{
    std::cout << "send threaad on\n";
    Socket *save;
    while (true) {
        try
        {
            std::lock_guard<std::mutex> lock(_loop);
            for (MyTree<Socket>::Iterator it = _clients.begin(); it.hasNext();) {
                Socket &now = it.next();
                save = &now;
                now.sendMsg();
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            delClient(save->getFD());
        }
        std::this_thread::yield();
    }
}
