#include "Server.hpp"

int main(void)
{
    Server server;
    // 서버 시동 전 초기화
    server.init();
    // 서버 시동
    server.run();
    return 0;
}