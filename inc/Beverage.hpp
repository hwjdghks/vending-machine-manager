#ifndef BEVERAGE_HPP
#define BEVERAGE_HPP
#include <string>

class Beverage
{
private:
    std::string     _name;  // Name of beverage
    int             _price; // price of beverage

public:
    Beverage(const std::string &name, int price);
    ~Beverage();

public:
    std::string getName() const noexcept;
    int getPrice() const noexcept;

public:
    void setName(const std::string &name);
    void setPrice(int price);

public:
    Beverage *clone(void) const;
};

#endif
