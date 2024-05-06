#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Critical.hpp"
#include "Buffer.hpp"
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <chrono>
#include <thread>
#include <functional>
#include <cstring>
#include <unistd.h>
#include <iostream>
#define BUFSIZE 1024 /* Need Edit */

class Client
{
private:
    int _fd;
    Buffer _readBuf;
    Buffer _writeBuf;
    Critical _mtxRecv;
    Critical _mtxSend;
    Critical _mtxEtc;

public:
    Client();
    ~Client();

public:
    void connect(void);
    void recvMsg(void);
    void sendMsg(void);

private:
    void _recv(void);
    void _send(void);
    int _getFd(void);
    void _setFd(int fd);
};

#endif
