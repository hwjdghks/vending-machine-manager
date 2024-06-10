#ifndef MYTREEITERATOR_HPP
#define MYTREEITERATOR_HPP

#include "MyTree.hpp"
#include "MyStack.hpp"

template <typename T>
class MyTree<T>::Iterator
{
private:
    MyStack<MyTreeNode<T> *> _stack;

public:
    Iterator(MyTreeNode<T> *root);
    bool hasNext(void) const;
    T &next(void);

private:
    void pushAllLeftNode(MyTreeNode<T> *node);
};

template<typename T>
MyTree<T>::Iterator::Iterator(MyTreeNode<T> *root)
{
    pushAllLeftNode(root);
}


template<typename T>
bool MyTree<T>::Iterator::hasNext(void) const
{
    return !_stack.empty();
}

template<typename T>
T &MyTree<T>::Iterator::next(void)
{
    MyTreeNode<T> *node = _stack.top();
    _stack.pop();
    pushAllLeftNode(node->_right);
    return node->_data;
}

template<typename T>
void MyTree<T>::Iterator::pushAllLeftNode(MyTreeNode<T> *node)
{
    while (node) {
        _stack.push(node);
        node = node->_left;
    }
}

#endif
