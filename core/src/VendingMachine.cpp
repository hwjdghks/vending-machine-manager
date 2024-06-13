#include "VendingMachine.hpp"

VendingMachine::VendingMachine()
: _rack{
    Shelf(0, "물", 450),
    Shelf(1, "커피", 500),
    Shelf(2, "이온음료", 550),
    Shelf(3, "고급커피", 700),
    Shelf(4, "탄산음료", 750),
    Shelf(5, "특화음료", 800)
  },
  _cashBox{
    CashTray(0, 10),
    CashTray(1, 50),
    CashTray(2, 100),
    CashTray(3, 500),
    CashTray(4, 1000)
  },
  _remainBalance(0),
  _noChangeAlert(false),
  _max1000won(0)
{}

VendingMachine::~VendingMachine() {}

/*
 * 상품 판매 함수
 */
void VendingMachine::sell(int id)
{
    _rack[id].sell();
    _remainBalance -= _rack[id].getPrice();
    // 투입한 돈을 알맞게 다 썼을 경우 한 사람의 구매가 끝난 것으로 생각
    // 1000원 투입 금액 개수 초기화
    if (_remainBalance == 0)
        _max1000won = 0;
    checkChange();
}

/*
 * 화폐 투입 함수
 */
void VendingMachine::deposit(int id)
{
    _cashBox[id].deposit();
    _remainBalance += _cashBox[id].getLabel();
    // 1000원 지폐 투입 개수 증가
    if (id == 4)
        _max1000won++;
}

/*
 * 회폐 반환 함수
 */
void VendingMachine::withdraw(int id)
{
    _cashBox[id].withdraw();
    _remainBalance -= _cashBox[id].getLabel();
}

/*
 * 매출 금액 수금 함수
 */
void VendingMachine::settlement(void)
{
    int total_money = 0;

    for (int i = 0; i < 5; i++)
        total_money += _cashBox[i].settlement();
    DebugLog::AddLog("수금한 총 금액 %d원", total_money);
}

/*
 * 투입 금액 반환 함수
 * 투입한 금액 중 사용하고 남은 금액을 반환한다
 * 만약 거스름돈이 모자랄 경우 예외를 발생시켜 알린다
 * 반환이 끝나면 다음 사람을 받기 위해 1000원권 지폐 투입 개수를 초기화 한다
 */
void VendingMachine::returnChange(void)
{
    // 화폐 단위
    int currency[5] = { 10, 50, 100, 500, 1000 };
    // 사용한 화폐
    int currency_used[5] = { 0 };
    // 보유 화폐
    int currency_amount[5] = { 0 };

    // 보유 화폐 초기화
    for (int i = 0; i < 5; i++)
        currency_amount[i] = _cashBox[i].getAmount();

    int i = 4;
    int temp_money = _remainBalance;
    // 거스름돈 계산
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
    // 거스름돈 계산 후 남은 돈이 있을 경우, 잔돈이 없어 전부 거슬러 주지 못하므로 예외 발생
    if (temp_money > 0)
        throw std::logic_error("거스름돈 부족! 반환 불가");
    else {
        DebugLog::AddLog("총액 %d원 반환",  _remainBalance);
        _remainBalance = 0;
    }
    // 계산을 토대로 거스를 화폐 개수반큼 반환
    for (int i = 0; i < 5; i++) {
        for (int used = currency_used[i]; used; used--) {
            _cashBox[i].withdraw();
        }
    }
    // 잔돈 유무 계산
    checkChange();
    // 1000원 투입 금액 개수 초기화
    _max1000won = 0;

    // 로그 출력
    std::string buffer;

    buffer = concatenate(currency_used[0], ", ",
                        currency_used[1], ", ",
                        currency_used[2], ", ",
                        currency_used[3], ", ",
                        currency_used[4]);
    DebugLog::AddLog("반환된 화폐 개수: %s", buffer.c_str());

    buffer = concatenate(_cashBox[0].getAmount(), ", ",
                        _cashBox[1].getAmount(), ", ",
                        _cashBox[2].getAmount(), ", ",
                        _cashBox[3].getAmount(), ", ",
                        _cashBox[4].getAmount());
    DebugLog::AddLog("남은 화폐 개수  : %s", buffer.c_str());
}

/*
 * 남은 화폐 중 어느 종류라도 3개 이하면 거스름돈 부족 알람 출력
 */
void VendingMachine::checkChange(void) noexcept
{
    bool flag = false;

    for (int i = 0; i < 5; i++)
        if (_cashBox[i].getAmount() < 3)
            flag = true;
    _noChangeAlert = flag;
}

std::string VendingMachine::getLabel(void) const noexcept
{
    return _label;
}

/*
 * 번호에 맞는 진열대를 반환하는 함수
 */
Shelf& VendingMachine::getRack(int idx)
{
    if (idx < 0 || idx > 5)
        throw std::logic_error("_rack 잘못된 인덱스 접근" + std::to_string(idx));
    return _rack[idx];
}

/*
 * 번호에 맞는 화폐 보관함을 반환하는 함수
 */
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

bool VendingMachine::getAlert(void) const noexcept
{
    return _noChangeAlert;
}

int VendingMachine::getMax1000won(void) const noexcept
{
    return _max1000won;
}

void VendingMachine::setLabel(const std::string label)
{
    if (label == "")
        throw std::logic_error("허용되지 않는 라벨! Before:" + getLabel() + ", After:" + label);
    _label = label;
}
