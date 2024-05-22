#include "Beverage.hpp"

Beverage::Beverage(const std::string &name, int price)
: _name(name), _price(price) {}

Beverage::~Beverage() {}

std::string Beverage::getName(void) const noexcept
{
    return _name;
}

int Beverage::getPrice(void) const noexcept
{
    return _price;
}

void Beverage::setName(const std::string &name)
{
    _name = name;
}

void Beverage::setPrice(int price)
{
    _price = price;
}

Beverage *Beverage::clone(void) const
{
    return new Beverage(getName(), getPrice());
}