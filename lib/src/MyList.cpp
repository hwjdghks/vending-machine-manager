#include "MyList.hpp"

template<typename T>
MyList<T>::MyList() : _head(nullptr), _tail(nullptr), _size(0) {}

template<typename T>
MyList<T>::~MyList()
{
    MyNode<T> *del = this->_head;
    MyNode<T> *next = nullptr;

    while (del != nullptr) {
        next = del->_next;
        delete del;
        del = next;
    }
}

template<typename T>
T MyList<T>::front(void)
{
    return _head->_data;
}

template<typename T>
T MyList<T>::back(void)
{
    return _tail->_data;
}

template<typename T>
void MyList<T>::push_back(T data)
{
    MyNode<T> *newNode = new MyNode(data);
    if (_head == nullptr) {
        _head = newNode;
        _tail = newNode;
    }
    else {
        _tail->_next = newNode;
        newNode->_prev = _tail;
        _tail = newNode;
    }
    _size++;
}

template<typename T>
void MyList<T>::pop_back(void)
{
    if (this->empty())
        throw std::exception(); /* Need Edit */

    MyNode<T> *delNode = nullptr;

    if (_head == _tail) {
        delNode = _tail;
        _head = nullptr;
        _tail = nullptr;
    }
    else {
        delNode = _tail;
        _tail = _tail->_prev;
        _tail->_next = nullptr;
    }
    delete delNode;
    _size--;
}

template<typename T>
void MyList<T>::pop_front(void)
{
    if (this->empty())
        throw std::exception(); /* Need Edit */

    MyNode<T> *delNode = nullptr;

    if (_head == _tail) {
        delNode = _head;
        _head = nullptr;
        _tail = nullptr;
    }
    else {
        delNode = _head;
        _head = _head->_next;
        _head->_prev = nullptr;
    }
    delete delNode;
    _size--;
}

template<typename T>
bool MyList<T>::empty(void) const noexcept
{
    if (_head == nullptr && _tail == nullptr)
        return true;
    return false;
}

template<typename T>
std::size_t MyList<T>::size(void) const noexcept
{
    return _size;
}
