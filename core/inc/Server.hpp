#ifndef SERVER_HPP
#define SERVER_HPP

#include "MyList.hpp"
#include "Socket.hpp"

class Server : public Socket
{
private:
    int             _fd;
    MyList<Socket>  _clients;

public:
    Server();
    ~Server();

public:
    void init(void);
    void run(void);

private:
    void acceptClient(void);
    void addClient(int fd);
    void getClient(int fd);
    void delClient(int fd);

private:
    void acceptLoop(void);
    void recvLoop(void);
    void sendLoop(void);
};
#endif
