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

#include "MyQueue.hpp"
#endif
