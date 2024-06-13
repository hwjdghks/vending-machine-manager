#include "Shelf.hpp"

Shelf::Shelf(int id, const std::string &label, int price)
: _id(id), _label(label), _price(price)
{
    // 상품을 초기 개수만큼 채워 넣는다
    for (int i = 0; i < PRODUCT_AMOUNT; i++)
        _shelf.push(Beverage(_label, _price));
}

Shelf::~Shelf() {}

/*
 * 상품 보충 함수
 */
void Shelf::refill(int count)
{
    for (int i = 0; i < count; i++)
        _shelf.push(Beverage(getLabel(), getPrice()));
}

/*
 * 상품 판매 함수
 */
void Shelf::sell(void)
{
    if (_shelf.empty())
        throw std::runtime_error("상품명 :" + getLabel() + ": 재고 없음!");
    _shelf.pop();
}

/*
 * 진열대에서 상품을 전부 빼는 함수
 * 몇개를 뺐는지 반환한다
 */
int Shelf::discard(void)
{
    int count = 0;
    while (!_shelf.empty()) {
        _shelf.pop();
        count++;
    }
    return count;
}

/*
 * 상품명과 가격을 업데이트하는 함수
 * 정보 업데이트 이후 진열대의 상품을 새로 고친다
 */
void Shelf::updataInfo(const std::string &label, int price)
{
    try // 상품명 변경 시도
    {
        setLabel(label);
    }
    catch(const std::logic_error& e)
    {
        DebugLog::AddLog("%s", e.what());
    }
    try // 상품 가격 변경 시도
    {
        setPrice(price);
    }
    catch(const std::logic_error& e)
    {
        DebugLog::AddLog("%s", e.what());
    }
    // 상품 업데이트
    refill(discard());
}

int Shelf::getID(void) const noexcept
{
    return _id;
}

std::string Shelf::getLabel(void) const noexcept
{
    return _label;
}

const char *Shelf::getLabelCstring(void) const noexcept
{
    return _label.c_str();
}

int Shelf::getPrice(void) const noexcept
{
    return _price;
}

std::size_t Shelf::getAmount(void) const noexcept
{
    return _shelf.size();
}

/*
 * 상품명을 설정하는 함수
 * 빈 문자열이면 에러 발생
 */
void Shelf::setLabel(const std::string &label)
{
    if (label == "")
        throw std::logic_error("허용되지 않는 라벨! Before:" + getLabel() + ", After:" + label);
    _label = label;
}

/*
 * 가격을 설정하는 함수
 * 가격은 10으로 나누어 떨어지는 자연수 중, 7000 이하로 제한
 */
void Shelf::setPrice(int price)
{
    if (price <= 0 || price > 7000 || (price % 10 != 0))
        throw std::logic_error("허용되지 않는 금액! Before:" + std::to_string(getPrice()) + ", After:" + std::to_string(price));
    _price = price;
}