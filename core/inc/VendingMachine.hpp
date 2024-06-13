#ifndef VENDINGMACHINE_HPP
#define VENDINGMACHINE_HPP

#include "Shelf.hpp"
#include "CashTray.hpp"

/*
 * 자판기 구성 클래스
 * 자판기 이름, 상품 진열대, 화폐 보관함, 잔돈 표기 등 여러 요소로 구성
 */
class VendingMachine
{
private:
    std::string _label;
    Shelf       _rack[6];
    CashTray    _cashBox[5];
    int         _remainBalance;
    bool        _noChangeAlert;
    int         _max1000won;

public:
    VendingMachine();
    ~VendingMachine();

public: /* 자판기 상품 관리 함수 */
    void sell(int id);
    void refill(int id, int count);

public: /* 자판기 화폐 관리 함수 */
    void deposit(int id);
    void withdraw(int id);
    void settlement(void);
    void returnChange(void);
    void checkChange(void) noexcept;

public: /* get & set */
    std::string getLabel(void) const noexcept;
    Shelf &getRack(int idx);
    CashTray &getCashBox(int idx);
    int getBalance(void) const noexcept;
    bool getAlert(void) const noexcept;
    int getMax1000won(void) const noexcept;

    void setLabel(const std::string label);
};
#endif
