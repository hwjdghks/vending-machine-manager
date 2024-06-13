#ifndef SHELF_HPP
#define SHELF_HPP

#include "Beverage.hpp"
#include "MyQueue.hpp"
#include "DebugLog.hpp"

#define PRODUCT_AMOUNT 10

/* 상품 진열대 클래스 */
class Shelf
{
private:
    int                 _id;
    std::string         _label;
    int                 _price;
    MyQueue<Beverage>   _shelf; // 실제 상품 관리 자료구조

public:
    Shelf(int id, const std::string &label, int price);
    ~Shelf();

public: /* 상품 선반 함수 */
    void refill(int count);
    void sell(void);
    int discard(void);
    void updataInfo(const std::string &label, int price);

public: /* get & set */
    int getID(void) const noexcept;
    std::string getLabel(void) const noexcept;
    const char *getLabelCstring(void) const noexcept;
    int getPrice(void) const noexcept;
    std::size_t getAmount(void) const noexcept;

    void setLabel(const std::string &label);
    void setPrice(int price);
};

#endif
