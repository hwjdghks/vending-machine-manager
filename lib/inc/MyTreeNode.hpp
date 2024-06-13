#ifndef MYTREENODE_HPP
#define MYTREENODE_HPP

/* AVL Tree 자료구조의 기반이 되는 단위 노드 클래스 */
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
