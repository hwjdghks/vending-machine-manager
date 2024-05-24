#ifndef CASHTRAY_HPP
#define CASHTRAY_HPP

#include "Currency.hpp"
#include "MyStack.hpp"
#include "DebugLog.hpp"

#define CURRENCY_AMOUNT 10

class CashTray
{
private:
    int _id;
    int _label;
    MyStack<Currency> _cashBox;

public:
    CashTray();
    CashTray(int id, int label);
    ~CashTray();

public:
    void deposit(void);
    void withdraw(void);
    int settlement(void);

public:
    int getID(void) const noexcept;
    int getLabel(void) const noexcept;
    int getAmount(void) const noexcept;
};

#endif
