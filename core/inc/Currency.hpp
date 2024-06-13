#ifndef CURRENCY_HPP
#define CURRENCY_HPP

/* 화폐 클래스 */
class Currency
{
private:
    int _label;

public:
    Currency(int label) : _label(label) {}
    ~Currency() {}
};

#endif
