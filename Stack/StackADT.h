#ifndef CCC_STACKADT_H
#define CCC_STACKADT_H

template <typename T>
class StackADT {
public:
    virtual ~StackADT() = default;

    virtual void initializeStack() = 0;
    virtual bool isFull() const = 0;
    virtual bool isEmpty() const = 0;
    virtual void push(const T& element) = 0;
    virtual void pop() = 0;
    virtual T top() const = 0;
    virtual long long size() const = 0;
    virtual void print() const = 0;
};

#endif //CCC_STACKADT_H