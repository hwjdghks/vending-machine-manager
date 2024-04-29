#ifndef CURRENCY_HPP
#define CURRENCY_HPP

#include <string>
#include <map>

class Currency
{
private:
    std::map<std::string, int>  _types;     // type of currency
    std::map<std::string, int>  _stock;  // reserve of currency

public:
};

#endif
