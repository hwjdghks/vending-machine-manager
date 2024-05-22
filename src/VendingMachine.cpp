#include "VendingMachine.hpp"

VendingMachine::VendingMachine() : _closed(false)
{
    std::string item[6] = {
        "물",
        "커피",
        "이온음료",
        "고급커피",
        "탄산음료",
        "특화음료"
    };
    int price[6] = {
        450,
        500,
        550,
        700,
        750,
        800
    };
    for(int i = 0; i < 6; i++) {
        _slot[i] = Slot(i, item[i], price[i]);
        _slot[i].refill(10);
    }
}

VendingMachine::~VendingMachine() {}

std::string VendingMachine::getName() const
{
    return _name;
}

Slot (&VendingMachine::getSlot())[6]
{
    return _slot;
}

Client &VendingMachine::getClient()
{
    return _client;
}

bool VendingMachine::isClosed(void) const noexcept
{
    return _closed;
}

void VendingMachine::setName(const std::string &name)
{
    _name = name;
}

void VendingMachine::setClosed(void) noexcept
{
    _closed = true;
}