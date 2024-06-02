#ifndef SERVER_HPP
#define SERVER_HPP

#include "MyList.hpp"
#include "Socket.hpp"

class Server : public Socket
{
private:
    MyList<Socket> _clients;

public:
    Server();
    ~Server();

public:
    void init(void);
    void run(void);
};
#endif
