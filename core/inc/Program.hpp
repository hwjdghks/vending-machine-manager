#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "VendingMachine.hpp"
#include "Client.hpp"

/*
 * 프로그램 전체 데이터를 포함하는 클래스
 * 자판기와 클라이언트용 소켓, 관리자 비밀번호가 포함
 */
class Program
{
private:
    VendingMachine _machine;
    Client _client;
    std::string _password;

public:
    Program();
    ~Program();

public: /* 관리자 함수 */
    bool equalPassword(const char *str) const noexcept;
    bool validatePassword(const char *str) const noexcept;
    void changePassword(const char *str);

public: /* get */
    VendingMachine &getMachine(void) noexcept;
    Client &getClient(void) noexcept;
    std::string getPassword(void) const noexcept;
};

#endif
