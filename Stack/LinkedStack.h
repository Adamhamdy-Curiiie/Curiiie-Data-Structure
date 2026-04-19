#ifndef LINKED_STACK_H
#define LINKED_STACK_H

#include "StackADT.h"

template <typename T>
struct StackNode {
    T data;
    StackNode* next;
    StackNode() : data(T()), next(nullptr) {}
    StackNode(T val) : data(val), next(nullptr) {}
    ~StackNode() {
        std::cout << "Destroyed node: " << data << " at " << this << "\n";
    }
};

template <typename T>
class LinkedStack : public StackADT<T> {
private:
    StackNode<T>* topNode;
    int length;
    void copyStack(const LinkedStack<T>& other);

public:
    LinkedStack();
    LinkedStack(const LinkedStack<T>& other);
    LinkedStack<T>& operator=(const LinkedStack<T>& other);
    ~LinkedStack() override;

    // ── Core ADT ──────────────────────────────────────────────────────────────
    void initializeStack() override;
    bool isFull()  const override;   // always false — heap never full( untill out of Memo)
    bool isEmpty() const override;
    void push(const T& element) override;
    void pop() override;
    T top() const override;
    int  size() const override;
    void print() const override;
};

#include "LinkedStack.cpp"
#endif