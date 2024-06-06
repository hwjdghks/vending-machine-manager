#include "Server.hpp"

Server::Server() {}

Server::~Server() {}

void Server::init(void)
{
    int dummy = 1;

    Socket::init(); // 공통 초기화
    _addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &dummy, sizeof(dummy)) == -1)
        throw std::runtime_error("setsockopt() failed.");
    if (bind(_fd, reinterpret_cast<sockaddr *>(&_addr), sizeof(_addr)) == -1)
        throw std::runtime_error("bind() failed.");
    if (listen(_fd, BACKLOG) == -1)
        throw std::runtime_error("listen() failed.");
}

void Server::acceptClient(void)
{
    int client_fd;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    errno = 0;
    client_fd = accept(_fd, reinterpret_cast<sockaddr *>(&addr), &len);
    if (client_fd == -1 && (errno != EAGAIN && errno != EWOULDBLOCK))
        throw std::runtime_error("accept() failed.");
    if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1) {
        close(client_fd);
        throw std::runtime_error("fcntl() failed.");
    }
    /* 클라이언트 목록에 추가 */
    /* 연결 성공 메세지 버퍼에 추가(welcome) */
}
