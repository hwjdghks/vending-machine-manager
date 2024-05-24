#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Buffer.hpp"
#include "DebugLog.hpp"

#include <unistd.h> // close
#include <sys/socket.h> // socket
#include <arpa/inet.h> // inet
#include <fcntl.h> // fcntl
#include <cstring> // memset
#include <iostream> // cerr
#include <thread> // thread
#include <chrono> // time

#define CONNECTION_IP "127.0.0.1"
#define PORT 6667
#define BACKLOG 5
#define BUFSIZE 2048

class Client
{
private:
    int _fd;
    int _id;
    struct sockaddr_in _addr;
    Buffer readBuf;
    Buffer writeBuf;
    std::mutex _mutex;

public:
    Client();
    ~Client();

private:
    void init(void);
    int recvMsg(void);
    int sendMsg(void);

public:
    void serverInit(void);
    void clientInit(void);
    void run(void);

public:
    int getFD(void);
    void setFD(int fd);
    void closeFD(void);

public:
    void addToBuf(const std::string &str);
    void addToBuf(const char *str);

private:
    static void recvLoop(Client &client);
    static void sendLoop(Client &clinet);
};
#endif
