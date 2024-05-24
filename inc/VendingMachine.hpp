#ifndef VENDINGMACHINE_HPP
#define VENDINGMACHINE_HPP

#include "Shelf.hpp"
#include "CashTray.hpp"

class VendingMachine
{
private:
    std::string _label;
    Shelf       _rack[6];
    CashTray    _cashBox[5];
    int         _remainBalance;

public:
    VendingMachine();
    ~VendingMachine();

public:
    void sell(int id);
    void refill(int id, int count);

public:
    void deposit(int id);
    void withdraw(int id);
    void settlement(void);
    void returnCharge(void);

public:
    std::string getLabel(void) const noexcept;
    Shelf &getRack(int idx);
    CashTray &getCashBox(int idx);
    int getBalance(void) const noexcept;

public:
    void setLabel(const std::string label);
};
#endif
