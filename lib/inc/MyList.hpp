#ifndef MYLIST_HPP
#define MYLIST_HPP

#include "MyNode.hpp"
#include <string>
#include <stdexcept>

/* 템플릿을 사용한 이중 연결 리스트 자료구조 */
template <typename T>
class MyList
{
private:
    MyNode<T> *_head;
    MyNode<T> *_tail;
    int _size;

public:
    MyList();
    ~MyList();

public: /* 자료구조 기능 */
    void push_back(T val) noexcept;
    void pop_back(void) noexcept;
    void pop_front(void) noexcept;
    T front(void);
    T back(void);
    MyNode<T> *begin(void) const noexcept;
    MyNode<T> *end(void) const noexcept;
    int size(void) const noexcept;
    bool empty(void) const noexcept;
};

template <typename T>
MyList<T>::MyList() : _head(nullptr), _tail(nullptr), _size(0) {}

template<typename T>
MyList<T>::~MyList()
{
    MyNode<T> *current = _head;
    while (current != nullptr) {
        MyNode<T> *next = current->_next;
        delete current;
        current = next;
    }
}

template <typename T>
void MyList<T>::push_back(T val) noexcept
{
    MyNode<T> *newNode = new MyNode<T>(val);
    if (_head == nullptr)
    {
        _head = _tail = newNode;
    }
    else
    {
        _tail->_next = newNode;
        newNode->_prev = _tail;
        _tail = newNode;
    }
    _size++;
}

template <typename T>
void MyList<T>::pop_back() noexcept
{
    if (_tail != nullptr)
    {
        MyNode<T> *del = _tail;
        _tail = _tail->_prev;
        if (_tail != nullptr)
            _tail->_next = nullptr;
        else
            _head = nullptr;
        delete del;
        _size--;
    }
}

template <typename T>
void MyList<T>::pop_front() noexcept
{
    if (_head != nullptr)
    {
        MyNode<T> *del = _head;
        _head = _head->_next;
        if (_head != nullptr)
            _head->_prev = nullptr;
        else
            _tail = nullptr;
        delete del;
        _size--;
    }
}

template<typename T>
T MyList<T>::front(void)
{
    if (_head == nullptr)
        throw std::logic_error("front() 데이터 없음.");
    return _head->_data;
}

template <typename T>
T MyList<T>::back(void)
{
    if (_tail == nullptr)
        throw std::logic_error("back() 데이터 없음.");
    return _tail->_data;
}

template <typename T>
MyNode<T> *MyList<T>::begin(void) const noexcept
{
    return _head;
}

template <typename T>
MyNode<T> *MyList<T>::end(void) const noexcept
{
    return nullptr;
}

template <typename T>
int MyList<T>::size(void) const noexcept
{
    return _size;
}

template <typename T>
bool MyList<T>::empty(void) const noexcept
{
    return (_size == 0);
}

#endif
