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

public: /* 클라이언트용 함수 */
    void init(void);
    void tryConnect(void);
    void closeConnect(void);
    void run(void);
    bool isConnected(void);
    void changeState(void);

private: /* 멀티 스레드용 루프 함수 */
    void recvLoop(void);
    void sendLoop(void);
};
#endif
