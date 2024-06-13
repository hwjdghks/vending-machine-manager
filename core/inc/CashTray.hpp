#ifndef CASHTRAY_HPP
#define CASHTRAY_HPP

#include "Currency.hpp"
#include "MyStack.hpp"
#include "DebugLog.hpp"

#define CURRENCY_AMOUNT 10

/* 화폐 보관함 클래스 */
class CashTray
{
private:
    int _id;
    int _label;
    MyStack<Currency> _cashBox; // 실제 화폐 관리 자료구조

public:
    CashTray(int id, int label);
    ~CashTray();

public:
    void deposit(void);
    void withdraw(void);
    int settlement(void);

public: /* get */
    int getID(void) const noexcept;
    int getLabel(void) const noexcept;
    int getAmount(void) const noexcept;
};

#endif
