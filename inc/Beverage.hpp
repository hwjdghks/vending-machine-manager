#ifndef BEVERAGE_HPP
#define BEVERAGE_HPP

#include <string>

class Beverage
{
private:
    std::string     _label;  // Name of beverage
    int             _price; // price of beverage

public:
    Beverage(const std::string &label, int price) : _label(label), _price(price) {}
};

#endif
