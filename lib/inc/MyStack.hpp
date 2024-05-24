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

template <typename T>
MyStack<T>::MyStack() {}

template <typename T>
MyStack<T>::~MyStack() {}

template <typename T>
void MyStack<T>::push(T data)
{
    c.push_back(data);
}

template <typename T>
void MyStack<T>::pop(void)
{
    c.pop_back();
}

template <typename T>
T MyStack<T>::top(void)
{
    return c.back();
}

template <typename T>
bool MyStack<T>::empty(void) const noexcept
{
    return c.empty();
}

template <typename T>
std::size_t MyStack<T>::size(void) const noexcept
{
    return c.size();
}

#endif
