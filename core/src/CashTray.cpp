#include "CashTray.hpp"

CashTray::CashTray(int id, int label)
: _id(id), _label(label)
{
    // 초기 값으로 세팅
    for (int i = 0; i < CURRENCY_AMOUNT; i++)
        _cashBox.push(Currency(_label));
}

CashTray::~CashTray() {}

/*
 * 돈 입금 함수
 */
void CashTray::deposit(void)
{
    _cashBox.push(Currency(getLabel()));
}

/*
 * 돈 출금 함수
 */
void CashTray::withdraw(void)
{
    if (_cashBox.empty())
        throw std::logic_error(std::to_string(getLabel()) + "원 잔돈 없음!");
    _cashBox.pop();
}

/*
 * 자판기의 돈을 수금하는 함수
 * 초기 값으로 개수를 조정한다
 * 초기 값보다 많으면 수금하고, 적으면 보충한다
 * 수금 결과를 반환한다
 */
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
    // 수금 또는 보충한 금액 로그 출력
    if (count >= 0)
        DebugLog::AddLog("[수금] %d원 개수: %d", _label, count);
    else
        DebugLog::AddLog("[보충] %d원 개수: %d", _label, -count);
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
