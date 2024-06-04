#include "Server.hpp"

Server::Server() {}

Server::~Server() {}

void Server::init(void)
{
    int dummy = 1;

    init(); // 공통 초기화
    _addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &dummy, sizeof(dummy)) == -1)
        throw std::runtime_error("setsockopt() failed.");
    if (bind(_fd, reinterpret_cast<sockaddr *>(&_addr), sizeof(_addr)) == -1)
        throw std::runtime_error("bind() failed.");
    if (listen(_fd, BACKLOG) == -1)
        throw std::runtime_error("listen() failed.");
}
