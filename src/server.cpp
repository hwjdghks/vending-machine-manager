#include "Program.hpp"
#include <signal.h>

int main(void)
{
    Client client;
    signal(SIGABRT, SIG_IGN);
try
{
   client.serverInit();
    printf("시작");
    client.run();
    printf("종료");
}
catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
}

    
}