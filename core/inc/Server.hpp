#ifndef SERVER_HPP
#define SERVER_HPP

#include "MyTree.hpp"
#include "Socket.hpp"

/*
 * 서버 소켓 클래스
 * 기본 소켓 클래스를 상속받아 사용
 * 클라이언트를 Tree 자료구조로 저장
 * mutex를 사용하여 클라이언트 목록에 상호 배제 적용
 */
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

private: /* client control */
    int acceptClient(void);
    void addClient(int fd);
    Socket &getClient(int fd);
    void delClient(Socket &client);

private: /* 멀티 스레드용 루프 함수 */
    void acceptLoop(void);
    void recvLoop(void);
    void sendLoop(void);

private:
    void parse(std::string &str);
};
#endif
