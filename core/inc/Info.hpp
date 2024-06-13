#ifndef INFO_HPP
#define INFO_HPP

#include <string>
#include <sstream>
class Info
{
public:
    int         _id;
    std::string _label[6];
    int         _price[6];
    int         _stock[6];
    int         _salesTotal[6];

    Info(int id);

    bool operator>(const Info &rhs) const;
    bool operator<(const Info &rhs) const;
    bool operator==(const Info &rhs) const;
};

std::ostream &operator<<(std::ostream &os, Info &ref);
#endif
