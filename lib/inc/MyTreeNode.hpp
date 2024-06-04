#ifndef MYTREENODE_HPP
#define MYTREENODE_HPP

template <typename T>
class MyTreeNode
{
public:
    T               _data;
    MyTreeNode<T>   *_left;
    MyTreeNode<T>   *_right;
    int             _height;
public:
    MyTreeNode(T data) : _data(data), _left(nullptr), _right(nullptr), _height(1) {}
};

#endif
