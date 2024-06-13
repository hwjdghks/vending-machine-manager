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

/*
 * 서버와 클라이언트 소켓 중 공통 요소를 구현한 클래스
 * 소켓마다 read, write 버터가 포함되어 있으며 _fd변수의 상호 배제를 위한 mutex 포함
 */
class Socket
{
protected:
    int _fd;
    int _id;
    struct sockaddr_in _addr;
    Buffer readBuf;
    Buffer writeBuf;
    std::mutex _so_mutex;

protected:
    Socket();

public:
    Socket(int fd);
    Socket(const Socket &rhs);
    virtual ~Socket();

public: /* 소켓 프로그래밍 기본 함수 */
    virtual void init(void);
    void recvMsg(void);
    void sendMsg(void);

public: /* FD Control */
    int getFD(void);
    void setFD(int fd);
    void closeFD(void);

public: /* Buffer Control */
    void addToWrite(const std::string &str);
    void addToWrite(const char *str);
    void addToRead(const std::string &str);
    void addToRead(const char *str);
    std::string getFromWrite(void);
    std::string getFromRead(void);

public: /* operator overloading */
    bool operator>(const Socket &rhs) const;
    bool operator<(const Socket &rhs) const;
    bool operator==(const Socket &rhs) const;
    Socket &operator=(const Socket &rhs);
};
#endif
