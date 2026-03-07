#ifndef STACK_H
#define STACK_H

#include <iostream>

template <typename T>
class Stack {
private:
    int size;
    int capacity;
    T*  data;

    void expand_capacity();

public:
    Stack(int capacity = 10);
    ~Stack();

    void push(T element);
    void pop();
    T    top() const;

    bool is_empty()  const { return size == 0; }
    int  get_size()  const { return size; }
};

#include "DodoStack.cpp"
#endif // STACK_H
