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
    MyTreeNode<T> *_root;

public:
    MyTree();
    ~MyTree();

public:
    void insertNode(T data);
    void deleteNode(T data);
    MyTreeNode<T> *findNode(int id);

public:
    class Iterator;
    Iterator begin(void) const;

private:
    MyTreeNode<T> *insertNode(MyTreeNode<T> *node, T data);
    MyTreeNode<T> *deleteNode(MyTreeNode<T> *node, T data);
    int getHeight(MyTreeNode<T> *node) const;
    int updateHeight(MyTreeNode<T> *node);
    int getBalance(MyTreeNode<T> *node);
    MyTreeNode<T> *getMinNode(MyTreeNode<T> *node);

private:
    MyTreeNode<T> *leftRotate(MyTreeNode<T> *node);
    MyTreeNode<T> *rightRotate(MyTreeNode<T> *node);

private:
    int max(int a, int b);
    MyTreeNode<T> *findNode(MyTreeNode<T> *node, T data);
};

template<typename T>
MyTree<T>::MyTree() : _root(nullptr) {}

template<typename T>
MyTree<T>::~MyTree() {}

template<typename T>
void MyTree<T>::insertNode(T data)
{
    _root = insertNode(_root, data);
}

template<typename T>
void MyTree<T>::deleteNode(T data)
{
    _root = deleteNode(_root, data);
}

template<typename T>
MyTreeNode<T>* MyTree<T>::insertNode(MyTreeNode<T> *node, T data)
{
    if (!node)
        return new MyTreeNode<T>(data);
    if (data < node->_data)
        node->_left = insertNode(node->_left, data);
    else if (data > node->_data)
        node->_right = insertNode(node->_right, data);
    else
        return node;
    node->_height = 1 + max(getHeight(node->_left), getHeight(node->_right));

    int balance = getBalance(node);
    // LL
    if (balance > 1 && data < node->_left->_data)
        return rightRotate(node);
    // RR
    if (balance < -1 && data < node->_right->_data)
        return leftRotate(node);
    // LR
    if (balance > 1 &&  data > node->_left->_data) {
        node->_left = leftRotate(node->_left);
        return rightRotate(node);
    }
    // RL
    if (balance < -1 &&  data < node->_right->_data) {
        node->_right = rightRotate(node->_right);
        return leftRotate(node);
    }
    return node;
}

template<typename T>
MyTreeNode<T>* MyTree<T>::deleteNode(MyTreeNode<T> *node, T data)
{
    if (node == nullptr)
        return node;
    if (data < node->_data)
        node->_left = deleteNode(node->_left, data);
    else if (data > node->_data)
        node->_right = deleteNode(node->_right, data);
    else {
        if (node->_left == nullptr || node->_right == nullptr) {
            MyTreeNode<T> *temp = node->_left ? node->_left : node->_right;
            if (temp == nullptr) {
                temp = node;
                node = nullptr;
            }
            else
                *node = *temp;
            delete temp;
        }
        else {
            MyTreeNode<T> *temp = getMinNode(node->_right);
            node->_data = temp->_data;
            node->_right = deleteNode(node->_right, node->_data);
        }
    }
    if (node == nullptr)
        return node;

    node->_height = 1 + max(getHeight(node->_left), getHeight(node->_right));

    int balance = getBalance(node);
    if (balance > 1 && getBalance(node->_left) >= 0)
        return rightRotate(node);

    if (balance > 1 && getBalance(node->_left) < 0) {
        node->_left = leftRotate(node->_left);
        return rightRotate(node);
    }

    if (balance < -1 && getBalance(node->_right) <= 0)
        return leftRotate(node);

    if (balance < -1 && getBalance(node->_right) > 0) {
        node->_right = rightRotate(node->_right);
        return leftRotate(node);
    }
    return node;
}

template<typename T>
MyTreeNode<T> *MyTree<T>::findNode(int id)
{
    return findNode(_root, T(id));
}

template<typename T>
typename MyTree<T>::Iterator MyTree<T>::begin(void) const{
    return Iterator(_root);
}

template<typename T>
int MyTree<T>::getHeight(MyTreeNode<T> *node) const
{
    if (node == nullptr)
        return 0;
    return node->_height;
}

template<typename T>
int MyTree<T>::getBalance(MyTreeNode<T> *node)
{
    if (node == nullptr)
        return 0;
    return getHeight(node->_left) - getHeight(node->_right);
}

template<typename T>
MyTreeNode<T> *MyTree<T>::getMinNode(MyTreeNode<T> *node)
{
    MyTreeNode<T> *current = node;

    while (current->_left != nullptr)
        current = current->_left;
    return current;
}

template<typename T>
MyTreeNode<T> *MyTree<T>::leftRotate(MyTreeNode<T> *node)
{
    MyTreeNode<T> *right = node->_right;
    MyTreeNode<T> *right_left = right->_left;

    right->_left = node;
    node->_right = right_left;
    node->_height = 1 + max(getHeight(node->_left), getHeight(node->_right));
    right->_height = 1 + max(getHeight(right->_left), getHeight(right->_left));
    return right;
}

template<typename T>
MyTreeNode<T> *MyTree<T>::rightRotate(MyTreeNode<T> *node)
{
    MyTreeNode<T> *left = node->_left;
    MyTreeNode<T> *left_right = left->_right;

    left->_right = node;
    node->_left = left_right;
    node->_height = 1 + max(getHeight(node->_left), getHeight(node->_right));
    left->_height = 1 + max(getHeight(left->_left), getHeight(left->_left));
    return left;
}

template<typename T>
int MyTree<T>::max(int a, int b)
{
    return (a > b) ? a : b;
}

template<typename T>
MyTreeNode<T> *MyTree<T>::findNode(MyTreeNode<T> *node, T data)
{
    if (node == nullptr || node->_data == data)
        return node;
    if (data < node->_data)
        return findNode(node->_left, data);
    else
        return findNode(node->_right, data);
}

#include "MyTreeIterator.hpp"
#endif
