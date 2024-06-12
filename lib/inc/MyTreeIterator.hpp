#ifndef MYTREEITERATOR_HPP
#define MYTREEITERATOR_HPP

#include "MyTree.hpp"
#include "MyStack.hpp"

/*
 * 트리 순회를 위한 반복자 클래스
 */
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

/*
 * 반복자 생성자
 */
template<typename T>
MyTree<T>::Iterator::Iterator(MyTreeNode<T> *root)
{
    pushAllLeftNode(root);
}

/*
 * 스택에 노드가 남았는지 확인
 */
template<typename T>
bool MyTree<T>::Iterator::hasNext(void) const
{
    return !_stack.empty();
}

/*
 * 저장된 노드를 반환한다
 * 해당 노드의 오른쪽 서브트리의 데이터를 스택에 저장하여
 * 작은 데이터부터 반환할 수 있게 구현
 */
template<typename T>
T &MyTree<T>::Iterator::next(void)
{
    MyTreeNode<T> *node = _stack.top();
    _stack.pop();
    pushAllLeftNode(node->_right);
    return node->_data;
}

/*
 * 현재 트리의 왼쪽 자식값들을 저장
 * 함수 실행 후 현재 트리의 최소값이 스택의 최상단에 위치하게 된다
 */
template<typename T>
void MyTree<T>::Iterator::pushAllLeftNode(MyTreeNode<T> *node)
{
    while (node) {
        _stack.push(node);
        node = node->_left;
    }
}

#endif
