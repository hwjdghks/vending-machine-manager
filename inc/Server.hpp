#ifndef SERVER_HPP
#define SERVER_HPP

#include "Socket.hpp"

class Server : public Socket
{
public:
    Server();
    ~Server();

public:
    void init(void);
    void run(void);
};
#endif
