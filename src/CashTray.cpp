#include "CashTray.hpp"

CashTray::CashTray() {}

CashTray::CashTray(int id, int label)
: _id(id), _label(label)
{
    for (int i = 0; i < CURRENCY_AMOUNT; i++)
        _cashBox.push(Currency(_label));
}

CashTray::~CashTray() {}

void CashTray::deposit(void)
{
    _cashBox.push(Currency(getLabel()));
}

void CashTray::withdraw(void)
{
    if (_cashBox.empty())
        throw std::logic_error(std::to_string(getLabel()) + "원 잔돈 없음!");
    _cashBox.pop();
}

int CashTray::settlement(void)
{
    int count = 0;
    // 돈이 많으면 amount만 남기고 가져감
    while (_cashBox.size() > CURRENCY_AMOUNT) {
        withdraw();
        count++;
    }
    // 돈이 적으면 amount까지 채움
    while (_cashBox.size() < CURRENCY_AMOUNT) {
        deposit();
        count--;
    }
    return _label * count;
}

int CashTray::getID(void) const noexcept
{
    return _id;
}

int CashTray::getLabel(void) const noexcept
{
    return _label;
}

int CashTray::getAmount(void) const noexcept
{
    return _cashBox.size();
}
