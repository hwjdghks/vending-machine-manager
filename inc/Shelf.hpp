#ifndef SHELF_HPP
#define SHELF_HPP

#include "Beverage.hpp"
#include "MyQueue.hpp"
#include "DebugLog.hpp"

#define PRODUCT_AMOUNT 10

class Shelf
{
private:
    int                 _id;
    std::string         _label;
    int                 _price;
    MyQueue<Beverage>   _shelf;

public:
    Shelf(int id, const std::string &label, int price);
    ~Shelf();

public:
    void refill(int count);
    void sell(void);
    int discard(void);
    void updataInfo(const std::string &label, int price);

public:
    int getID(void) const noexcept;
    std::string getLabel(void) const noexcept;
    const char *getLabelCstring(void) const noexcept;
    int getPrice(void) const noexcept;
    std::size_t getAmount(void) const noexcept;

    void setLabel(const std::string &label);
    void setPrice(int price);
};

#endif
