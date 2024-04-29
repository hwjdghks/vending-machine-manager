#ifndef MYSTACK_HPP
#define MYSTACK_HPP

#include "MyList.hpp"

template <typename T>
class MyStack
{
private:
    MyList<T> c;

public:
    MyStack();
    ~MyStack();

public:
    T top(void);

public:
    void push(T data);
    void pop(void);

public:
    bool empty(void) const noexcept;
    std::size_t size(void) const noexcept;

public:
    /*
     * TODO
     * Add custom Exception
    */
};

#include "MyStack.hpp"
#endif
