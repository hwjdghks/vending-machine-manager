#include "VendingMachine.hpp"


VendingMachine::VendingMachine()
: _remainBalance(0)
{
    std::string label[6] = {
        "물",
        "커피",
        "이온음료",
        "고급커피",
        "탄산음료",
        "특화음료"
    };
    int price[6] = { 450, 500, 550, 700, 750, 800};
    int currency[5] = { 10, 50, 100, 500, 1000 };

    for (int i = 0; i < 6; i++)
        _rack[i] = Shelf(i, label[i], price[i]);
    for (int i = 0; i < 5; i++) 
        _cashBox[i] = CashTray(i, currency[i]);
    printf("머신 끝\n");
    printf("%ld\n", _rack[0].getAmount());
}

VendingMachine::~VendingMachine() {}

void VendingMachine::sell(int id)
{
    _rack[id].sell();
}

void VendingMachine::deposit(int id)
{
    _cashBox[id].deposit();
    _remainBalance += _cashBox[id].getLabel();
}

void VendingMachine::withdraw(int id)
{
    _cashBox[id].withdraw();
    _remainBalance -= _cashBox[id].getLabel();
}

void VendingMachine::settlement(void)
{
    int total_money = 0;

    for (int i = 0; i < 5; i++)
        total_money += _cashBox[i].settlement();
}

void VendingMachine::returnCharge(void)
{
    int currency[5] = { 10, 50, 100, 500, 1000 };
    int currency_used[5];
    int currency_amount[5];

    for (int i = 0; i < 5; i++)
        currency_amount[i] = _cashBox[i].getAmount();

    int i = 4;
    int temp_money = _remainBalance;

    while (temp_money > 0 && i >= 0) {
        int amount = temp_money / currency[i];
        if (amount <= currency_amount[i]) {
            temp_money -= amount * currency[i];
            currency_used[i] = amount;
        }
        else {
            temp_money -= currency_amount[i] * currency[i];
            currency_used[i] = currency_amount[i];
        }
        i--;
    }
    if (temp_money > 0)
        throw std::logic_error("잔돈 없음! 반환 불가!");
    else {
        DebugLog::AddLog("총액 %d원 반환",  _remainBalance);
        _remainBalance = 0;
    }
    for (int i = 0; i < 5; i++) {
        DebugLog::AddLog("%d원 %d개 반환.", currency[i], currency_used[i]);
        for (int used = currency_used[i]; used; used--)
            _cashBox[i].withdraw();
        DebugLog::AddLog("남은 %d원 개수: %d", currency[i], _cashBox[i].getAmount());
    }
}

std::string VendingMachine::getLabel(void) const noexcept
{
    return _label;
}

Shelf& VendingMachine::getRack(int idx)
{
    if (idx < 0 || idx > 5)
        throw std::logic_error("_rack 잘못된 인덱스 접근" + std::to_string(idx));
    return _rack[idx];
}

CashTray& VendingMachine::getCashBox(int idx)
{
    if (idx < 0 || idx > 4)
        throw std::logic_error("_cashBox 잘못된 인덱스 접근:" + std::to_string(idx));
    return _cashBox[idx];
}

int VendingMachine::getBalance(void) const noexcept
{
    return _remainBalance;
}

void VendingMachine::setLabel(const std::string label)
{
    if (label == "")
        throw std::logic_error("허용되지 않는 라벨! Before:" + getLabel() + ", After:" + label);
    _label = label;
}
