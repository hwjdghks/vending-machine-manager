#include "Slot.hpp"

Slot::Slot() 
: _ID(-1), _type(""), _price(-1) {}

Slot::Slot(int id, const std::string &type, int price)
: _ID(id), _type(type), _price(price) {}

Slot::~Slot() {}

int Slot::getID(void) const noexcept
{
    return _ID;
}

int Slot::getPrice(void) const noexcept
{
    return _price;
}

std::string Slot::getType(void) const noexcept
{
    return _type;
}

std::size_t Slot::getStock(void) const noexcept
{
    return _storage.size();
}

/*
 * 음료 정보(가격, 이름) 업데이트
*/
void Slot::updateInfo(int price, const std::string &type)
{
    if (price != -1) {
        _price = price;
        MyNode<Beverage> *p = _storage.begin(); // 리스트 순회
        while (p != nullptr) {
            p->_data.setPrice(_price);
            p = p->_next;
        }
    }
    if (type != "") {
        _type = type;
        MyNode<Beverage> *p = _storage.begin(); // 리스트 순회
        while (p != nullptr) {
            p->_data.setName(_type);
            p = p->_next;
        }
    }
}

void Slot::initStock()
{
    for (int i = 0; i < 10; i++)
        _storage.push_back(Beverage(getType(), getPrice()));
}

void Slot::refill(std::size_t stock)
{
    for(std::size_t i = 0; i < stock; i++)
        _storage.push_back(Beverage(getType(), getPrice()));
}

/*
 * 음료 판매
 * 리스트의 앞에서 제거 
 */
void Slot::sell(void)
{
    if (_storage.empty())
        throw std::logic_error(getType() + "재고 없음!");
    _storage.pop_front(); // 판매
}