#ifndef SLOT_HPP
#define SLOT_HPP

#include "MyQueue.hpp"

template <typename T>
class Slot
{
private:
    int         _ID;
    std::string _type;
    MyQueue<T>  _storage;

public:
    Slot();
    ~Slot();

public:
    int getID(void) const noexcept;
    std::string getType(void) const noexcept;
    std::size_t getStock(void) const noexcept;

public:
    void setType(const std::string &type);

public:
    void refill();  // 보충
    void dispense(); // 판매
};

#endif
