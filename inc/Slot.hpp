#ifndef SLOT_HPP
#define SLOT_HPP

#include "MyList.hpp"
#include "Beverage.hpp"

class Slot
{
private:
    int         _ID;
    std::string _type;
    int         _price;
    MyList<Beverage>  _storage;

public:
    Slot();
    Slot(int id, const std::string &type, int price);
    ~Slot();

public:
    int getID(void) const noexcept;
    int getPrice(void) const noexcept;
    std::string getType(void) const noexcept;
    std::size_t getStock(void) const noexcept;

public:
    void updateInfo(int price = -1, const std::string &type = "");

public:
    void initStock();
    void refill(std::size_t stock);  // 보충
    void sell();    // 판매
    void flush();   // 비우기
};

#endif
