#ifndef MYTREE_HPP
#define MYTREE_HPP

#include "MyTreeNode.hpp"

/*
 * AVL 트리 구현
 * 자동으로 정렬 가능
 */
template <typename T>
class MyTree
{
private:
    MyTreeNode<T> *root;

public:
    MyTree();
    ~MyTree();

public:
    void insertNode();
    void deleteNode();
    MyTreeNode *findNode();
};

template<typename T>
MyTree<T>::MyTree() : root(nullptr) {}

template<typename T>
MyTree<T>::~MyTree() {}


#endif
