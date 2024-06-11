#ifndef SOCKET_HPP
#define SOCKET_HPP

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

class Socket
{
protected:
    int _fd;
    int _id;
    struct sockaddr_in _addr;
    Buffer readBuf;
    Buffer writeBuf;
    std::mutex _mutex;

protected:
    Socket();

public:
    Socket(int fd);
    Socket(const Socket &rhs);
    virtual ~Socket();

    virtual void init(void);
    int recvMsg(void);
    int sendMsg(void);

public:
    int getFD(void);
    void setFD(int fd);
    void closeFD(void);

public:
    void addToWrite(const std::string &str);
    void addToWrite(const char *str);
    void addToRead(const std::string &str);
    void addToRead(const char *str);
    std::string getFromWrite(void);
    std::string getFromRead(void);

public:
    bool operator>(const Socket &rhs) const;
    bool operator<(const Socket &rhs) const;
    bool operator==(const Socket &rhs) const;
    Socket &operator=(const Socket &rhs);
};
#endif
