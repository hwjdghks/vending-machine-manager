#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Socket.hpp"

class Client : public Socket
{
private:
    bool _isConnect;

public:
    Client();
    ~Client();

public:
    void init(void);
    void tryConnect(void);
    void run(void);
    bool isConnected(void);
    void changeState(void);

private:
    void recvLoop(void);
    void sendLoop(void);
};
#endif
