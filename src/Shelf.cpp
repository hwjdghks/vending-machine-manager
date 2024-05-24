#include "Shelf.hpp"

Shelf::Shelf() { printf("생성\n"); }

Shelf::Shelf(int id, const std::string &label, int price)
: _id(id), _label(label), _price(price)
{
    printf("%d %s 생성\n", id, label.c_str());
    for (int i = 0; i < PRODUCT_AMOUNT; i++)
        _shelf.push(Beverage(_label, _price));
    printf("생성 끝\n");
}

Shelf::~Shelf() { printf("파괴\n"); }


void Shelf::refill(int count)
{
    for (int i = 0; i < count; i++)
        _shelf.push(Beverage(getLabel(), getPrice()));
}

void Shelf::sell(void)
{
    if (_shelf.empty())
        throw std::runtime_error("상품명 :" + getLabel() + ": 재고 없음!");
    _shelf.pop();
}

int Shelf::discard(void)
{
    int count = 0;
    while (!_shelf.empty()) {
        _shelf.pop();
        count++;
    }
    return count;
}

void Shelf::updataInfo(const std::string &label, int price)
{
    try
    {
        setLabel(label);
    }
    catch(const std::logic_error& e)
    {
        DebugLog::AddLog("%s", e.what());
    }
    try
    {
        setPrice(price);
    }
    catch(const std::logic_error& e)
    {
        DebugLog::AddLog("%s", e.what());
    }
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

void Shelf::setLabel(const std::string &label)
{
    if (label == "")
        throw std::logic_error("허용되지 않는 라벨! Before:" + getLabel() + ", After:" + label);
    _label = label;
}

void Shelf::setPrice(int price)
{
    if (price < 0)
        throw std::logic_error("허용되지 않는 금액! Before:" + getLabel() + ", After:" + std::to_string(price));
    _price = price;
}