#ifndef MYQUEUE_HPP
#define MYQUEUE_HPP

#include "MyList.hpp"


/*
 * list를 기반으로 구현된 큐 자료구조
 */
template <typename T>
class MyQueue
{
private:
    MyList<T> c; // 기반 자료구조

public:
    MyQueue();
    ~MyQueue();

public: /* 자료구조 기능 */
    T front(void);
    T back(void);
    void push(T data) noexcept;
    void pop(void) noexcept;
    bool empty(void) const noexcept;
    std::size_t size(void) const noexcept;
};


template <typename T>
MyQueue<T>::MyQueue() {}

template <typename T>
MyQueue<T>::~MyQueue() {}

template <typename T>
T MyQueue<T>::front(void)
{
    return c.front();
}

template <typename T>
T MyQueue<T>::back(void)
{
    return c.back();
}

template <typename T>
void MyQueue<T>::push(T data) noexcept
{
    c.push_back(data);
}

template <typename T>
void MyQueue<T>::pop(void) noexcept
{
    c.pop_front();
}

template <typename T>
bool MyQueue<T>::empty(void) const noexcept
{
    return c.empty();
}

template <typename T>
std::size_t MyQueue<T>::size(void) const noexcept
{
    return c.size();
}

#endif
