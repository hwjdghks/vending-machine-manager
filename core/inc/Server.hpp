#ifndef SERVER_HPP
#define SERVER_HPP

#include "MyTree.hpp"
#include "Socket.hpp"

class Server : public Socket
{
private:
    MyTree<Socket>  _clients;
    std::mutex      _s_mutex;
    std::mutex      _loop;
    int             _mode;

public:
    Server();
    ~Server();

public:
    void init(void);
    void run(void);
    void acceptLoop(void);

private:
    int acceptClient(void);
    void addClient(int fd);
    Socket &getClient(int fd);
    void delClient(Socket &client);

private:
    void recvLoop(void);
    void sendLoop(void);
};
#endif
