#ifndef STACK_H
#define STACK_H

#include <iostream>
#include "StackADT.h"

template <typename T>
class ArrayStack : public StackADT<T> {
private:
    T* data;
    long long topIdx;
    long long capacity;
    void copyStack(const ArrayStack<T>& other) ;// only to help other methods in same class

public:
    ArrayStack();
    ArrayStack(long long capacity);
    virtual ~ArrayStack();
    //deep copy
    ArrayStack(const ArrayStack<T>& other);
    ArrayStack<T>& operator=(const ArrayStack<T>& other);
    //move methods
    void moveStack(ArrayStack<T>&& other) noexcept;
    ArrayStack(ArrayStack<T>&& other) noexcept;
    ArrayStack<T>& operator=(ArrayStack<T>&& other) noexcept;


// CORE StackADT functions
     void initializeStack() override;
     bool isFull() const override;
     bool isEmpty() const override;
     void push(const T& element) override;
    void push(T&& element) ;
     void pop() override;
     T top() const override;
     long long size() const override;
    void print() const override;

};

#include "ArrayStack.cpp"
#endif
