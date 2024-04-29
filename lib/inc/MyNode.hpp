#ifndef MYNODE_HPP
#define MYNODE_HPP

template <typename T>
class MyNode
{
public:
    T           _data;
    MyNode<T>   *_prev;
    MyNode<T>   *_next;

public:
    MyNode(T data) : _data(data), _prev(nullptr), _next(nullptr) {}
};

#endif
