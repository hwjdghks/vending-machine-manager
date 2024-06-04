#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "VendingMachine.hpp"
#include "Client.hpp"

class Program
{
private:
    VendingMachine _machine;
    Client _client;
    std::string _password;

public:
    Program();
    ~Program();

public:
    bool equalPassword(const char *str) const noexcept;
    bool validatePassword(const char *str) const noexcept;
    void changePassword(const char *str);

public:
    VendingMachine &getMachine(void) noexcept;
    Client &getClient(void) noexcept;
    std::string getPassword(void) const noexcept;
};

#endif
