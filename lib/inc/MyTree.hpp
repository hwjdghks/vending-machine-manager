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

public: /* 자료구조 기능 */
    void insertNode(T data) noexcept;
    void deleteNode(T data) noexcept;
    MyTreeNode<T> *findNode(int id) noexcept;

public: /* 반복자 기능 */
    class Iterator;
    Iterator begin(void) const noexcept;

private: /* 트리 내부 함수 */
    MyTreeNode<T> *insertNode(MyTreeNode<T> *node, T data) noexcept;
    MyTreeNode<T> *deleteNode(MyTreeNode<T> *node, T data) noexcept;
    MyTreeNode<T> *findNode(MyTreeNode<T> *node, T data) noexcept;

private: /* get */
    int getHeight(MyTreeNode<T> *node) const noexcept;
    int updateHeight(MyTreeNode<T> *node) noexcept;
    int getBalance(MyTreeNode<T> *node) noexcept;
    MyTreeNode<T> *getMinNode(MyTreeNode<T> *node) noexcept;

private: /* Roatate Control*/
    MyTreeNode<T> *leftRotate(MyTreeNode<T> *node) noexcept;
    MyTreeNode<T> *rightRotate(MyTreeNode<T> *node) noexcept;

private: /* utility */
    int max(int a, int b) noexcept;
    void deleteTree(MyTreeNode<T> *node) noexcept;
};

template<typename T>
MyTree<T>::MyTree() : _root(nullptr) {}

template<typename T>
MyTree<T>::~MyTree()
{
    deleteTree(_root);
}

/*
 * 재귀적으로 순회하면서 노드를 삽입
 * 트리의 균형을 조절한 후 전체 트리의 root를 반환
 */
template<typename T>
void MyTree<T>::insertNode(T data) noexcept
{
    _root = insertNode(_root, data);
}

/*
 * 재귀적으로 순회하면서 노드를 제거
 * 트리의 균형을 조절한 후 전체 트리의 root를 반환
 */
template<typename T>
void MyTree<T>::deleteNode(T data) noexcept
{
    _root = deleteNode(_root, data);
}

/*
 * 재귀적으로 순회하면서 노드를 삽입
 */
template<typename T>
MyTreeNode<T>* MyTree<T>::insertNode(MyTreeNode<T> *node, T data) noexcept
{
    // 현재 위치에 새 노드 생성
    if (!node)
        return new MyTreeNode<T>(data);
    // 삽입 위치를 찾아 재귀적으로 진입하여 삽입
    if (data < node->_data)
        node->_left = insertNode(node->_left, data);
    else if (data > node->_data)
        node->_right = insertNode(node->_right, data);
    // 이미 존재할 경우 해당 노드 반환
    else
        return node;
    // 노드 높이 업데이트
    node->_height = 1 + max(getHeight(node->_left), getHeight(node->_right));
    // 현재 위치에서 불균형이 발생했는지 계산
    int balance = getBalance(node);
    // LL 경우
    if (balance > 1 && data < node->_left->_data)
        return rightRotate(node);
    // RR 경우
    if (balance < -1 && data < node->_right->_data)
        return leftRotate(node);
    // LR 경우
    if (balance > 1 &&  data > node->_left->_data) {
        node->_left = leftRotate(node->_left);
        return rightRotate(node);
    }
    // RL 경우
    if (balance < -1 &&  data < node->_right->_data) {
        node->_right = rightRotate(node->_right);
        return leftRotate(node);
    }
    // 균형을 조절한 후 현재 트리의 root를 반환
    return node;
}

template<typename T>
MyTreeNode<T>* MyTree<T>::deleteNode(MyTreeNode<T> *node, T data) noexcept
{
    // 삭제할 노드를 찾지 못했을 경우 nullptr 반환
    if (node == nullptr)
        return node;
    // 삭제할 노드를 찾아 재귀적으로 진입 후 삭제
    if (data < node->_data)
        node->_left = deleteNode(node->_left, data);
    else if (data > node->_data)
        node->_right = deleteNode(node->_right, data);
    // 삭제할 노드를 찾았을 경우
    else {
        // 자식이 1개 이하일 경우
        if (node->_left == nullptr || node->_right == nullptr) {
            // 자식을 temp로 설정
            MyTreeNode<T> *temp = node->_left ? node->_left : node->_right;
            // 자식이 없을 경우
            if (temp == nullptr) {
                temp = node;
                node = nullptr;
            }
            else // 자식이 있을 경우 자식 노드의 정보(링크 정보 포함)를 내 위치로 옮김
                *node = *temp;
                // 노드 삭제
            delete temp;
        }
        // 자식이 2개일 경우
        else {
            // 오른쪽 서브트리에서 최소값을 갖는 노드를 설정
            MyTreeNode<T> *temp = getMinNode(node->_right);
            // 데이터만 복사 (링크 정보 제외)
            node->_data = temp->_data;
            // 최소값을 갖는 노드 삭제
            node->_right = deleteNode(node->_right, node->_data);
        }
    }
    // 현재 노드가 nullptr일 경우 nullptr 반환
    if (node == nullptr)
        return node;
    // 노드 높이 업데이트
    node->_height = 1 + max(getHeight(node->_left), getHeight(node->_right));
    // 현재 위치에서 불균형이 발생했는지 계산
    int balance = getBalance(node);
    // LL 경우
    if (balance > 1 && getBalance(node->_left) >= 0)
        return rightRotate(node);
    // RR 경우
    if (balance < -1 && getBalance(node->_right) <= 0)
        return leftRotate(node);
    // LR 경우
    if (balance > 1 && getBalance(node->_left) < 0) {
        node->_left = leftRotate(node->_left);
        return rightRotate(node);
    }
    // RL 경우
    if (balance < -1 && getBalance(node->_right) > 0) {
        node->_right = rightRotate(node->_right);
        return leftRotate(node);
    }
    // 균형을 조절한 후 현재 트리의 root를 반환
    return node;
}

/*
 * 노드를 재귀적으로 순회하여 찾는 노드를 반환하는 함수
 */
template<typename T>
MyTreeNode<T> *MyTree<T>::findNode(MyTreeNode<T> *node, T data) noexcept
{
    if (node == nullptr || node->_data == data)
        return node;
    if (data < node->_data)
        return findNode(node->_left, data);
    else
        return findNode(node->_right, data);
}

/*
 * 이진 탐색으로 노드를 찾는 함수
 */
template<typename T>
MyTreeNode<T> *MyTree<T>::findNode(int id) noexcept
{
    return findNode(_root, T(id));
}

/*
 * 전체 트리에 대해서 반복자 생성
 */
template<typename T>
typename MyTree<T>::Iterator MyTree<T>::begin(void) const noexcept
{
    return Iterator(_root);
}

/*
 * 현재 노드의 높이를 반환
 */
template<typename T>
int MyTree<T>::getHeight(MyTreeNode<T> *node) const noexcept
{
    if (node == nullptr)
        return 0;
    return node->_height;
}

/*
 * 현재 노드의 높이 차를 계산하여 반환
 * 결과가 -1 미만 또는 1 초과일 경우 해당 노드를 기준으로 트리 불균형이 발생
 */
template<typename T>
int MyTree<T>::getBalance(MyTreeNode<T> *node) noexcept
{
    if (node == nullptr)
        return 0;
    return getHeight(node->_left) - getHeight(node->_right);
}

/*
 * 최소값을 갖는 노드 반환
 * 트리의 구조상 최소값은 왼쪽 아래에 있으므로 해당 노드를 찾아서 반환한다
 */
template<typename T>
MyTreeNode<T> *MyTree<T>::getMinNode(MyTreeNode<T> *node) noexcept
{
    MyTreeNode<T> *current = node;

    while (current->_left != nullptr)
        current = current->_left;
    return current;
}

/*
 * 왼쪽 회전 함수
 */
template<typename T>
MyTreeNode<T> *MyTree<T>::leftRotate(MyTreeNode<T> *node) noexcept
{
    MyTreeNode<T> *right = node->_right;
    MyTreeNode<T> *right_left = right->_left;

    right->_left = node;
    node->_right = right_left;
    node->_height = 1 + max(getHeight(node->_left), getHeight(node->_right));
    right->_height = 1 + max(getHeight(right->_left), getHeight(right->_left));
    // 회전 후 서브트리의 root를 반환
    return right;
}

/*
 * 오른쪽 회전 함수
 */
template<typename T>
MyTreeNode<T> *MyTree<T>::rightRotate(MyTreeNode<T> *node) noexcept
{
    MyTreeNode<T> *left = node->_left;
    MyTreeNode<T> *left_right = left->_right;

    left->_right = node;
    node->_left = left_right;
    node->_height = 1 + max(getHeight(node->_left), getHeight(node->_right));
    left->_height = 1 + max(getHeight(left->_left), getHeight(left->_left));
    // 회전 후 서브트리의 root를 반환
    return left;
}

/*
 * std::max를 사용하기 위해 필요한 헤더 파일을 include하는 대신 직접 구현
 */
template<typename T>
int MyTree<T>::max(int a, int b) noexcept
{
    return (a > b) ? a : b;
}

/*
 * 트리 전체 삭제 함수
 */
template<typename T>
void MyTree<T>::deleteTree(MyTreeNode<T> *node) noexcept {
    if (node != nullptr) {
        deleteTree(node->_left);
        deleteTree(node->_right);
        delete node;
    }
}

#include "MyTreeIterator.hpp"
#endif
