#ifndef MYSTACK_HPP
#define MYSTACK_HPP

#include "MyList.hpp"

/*
 * list를 기반으로 구현된 스택 자료구조
 */
template <typename T>
class MyStack
{
private:
    MyList<T> c; // 기반 자료구조

public:
    MyStack();
    ~MyStack();

public: /* 자료구조 기능 */
    T top(void);
    void push(T data) noexcept;
    void pop(void) noexcept;
    bool empty(void) const noexcept;
    std::size_t size(void) const noexcept;
};

template <typename T>
MyStack<T>::MyStack() {}

template <typename T>
MyStack<T>::~MyStack() {}

template <typename T>
void MyStack<T>::push(T data) noexcept
{
    c.push_back(data);
}

template <typename T>
void MyStack<T>::pop(void) noexcept
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
