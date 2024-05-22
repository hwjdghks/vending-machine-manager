#ifndef MYLIST_HPP
#define MYLIST_HPP

#include "MyNode.hpp"
#include <string>
#include <stdexcept>

template <typename T>
class MyList
{
private:
    MyNode<T>   *_head;
    MyNode<T>   *_tail;
    std::size_t _size;

public:
    MyList();
    ~MyList();

public:
    MyNode<T> *begin(void);

public:
    T front(void);
    T back(void);

public:
    void push_back(T data);
    void pop_front(void);
    void pop_back(void);

public:
    bool empty(void) const noexcept;
    std::size_t size() const noexcept;

public:
    /*
     * TODO
     * Add custom Exception
    */
};

#include "MyList.cpp"
#endif
