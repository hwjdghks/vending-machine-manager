#ifndef MYNODE_HPP
#define MYNODE_HPP

/* 자료구조의 기반이 되는 단위 노드 클래스 */
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
