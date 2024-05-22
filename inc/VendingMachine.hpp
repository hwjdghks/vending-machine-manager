#ifndef VENDINGMACHINE_HPP
#define VENDINGMACHINE_HPP

#include "Beverage.hpp"
#include "Slot.hpp"
#include "Currency.hpp"
#include "Client.hpp"

class VendingMachine
{
private:
    std::string _name;
    Slot        _slot[6];
    Client      _client;
    bool        _closed;

public:
    VendingMachine();
    ~VendingMachine();

public:
    std::string getName() const;
    Slot (&getSlot())[6];
    Client &getClient();
    bool isClosed() const noexcept;

    void setName(const std::string &name);
    void setClosed(void) noexcept;
};


#endif
