#ifndef BEVERAGE_HPP
#define BEVERAGE_HPP

#include <string>

/* 상품 클래스 */
class Beverage
{
private:
    std::string     _label;
    int             _price;

public:
    Beverage(const std::string &label, int price) : _label(label), _price(price) {}
};

#endif
