#ifndef MYQUEUE_HPP
#define MYQUEUE_HPP

#include "MyList.hpp"

template <typename T>
class MyQueue
{
private:
    MyList<T> c;

public:
    MyQueue();
    ~MyQueue();

public:
    T front(void);
    T back(void);

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
void MyQueue<T>::push(T data)
{
    c.push_back(data);
}

template <typename T>
void MyQueue<T>::pop(void)
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
