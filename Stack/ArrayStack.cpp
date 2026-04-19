#include "ArrayStack.h"
#include <iostream>
#include <cassert>

template <typename T>
ArrayStack<T>::ArrayStack() {
    topIdx = 0;
    capacity = 500;
    data = new T[capacity];
}

template <typename T>
ArrayStack<T>::~ArrayStack() {
    delete [] data;
}

template <typename T>
ArrayStack<T>::ArrayStack(long long capacity) {
    assert(capacity >= 0);
    topIdx = 0;
    this->capacity = capacity;
    data = new T[capacity];
}

template<typename T>
ArrayStack<T>::ArrayStack(const ArrayStack<T> &other) : data(nullptr), topIdx(0), capacity(0){
    copyStack(other);
}

template<typename T>
ArrayStack<T>& ArrayStack<T>::operator=(const ArrayStack<T> &other) {
    if (this != &other) {
        copyStack(other);
    }
    return *this;
}

template <typename T>
void ArrayStack<T>::copyStack(const ArrayStack<T>& other) {
    delete[] data;
    topIdx = other.topIdx;
    capacity = other.capacity;
    data = new T[capacity];

    for (long long i = 0; i < other.topIdx; i++) {
        data[i] = other.data[i];
    }
}

template<typename T>
ArrayStack<T>::ArrayStack(ArrayStack<T>&& other) noexcept
    : data(nullptr), topIdx(0), capacity(0) {
    moveStack(std::move(other));
}

template<typename T>
ArrayStack<T>& ArrayStack<T>::operator=(ArrayStack<T>&& other) noexcept {
    if (this != &other) {
        moveStack(std::move(other));
    }
    return *this;
}

template<typename T>
void ArrayStack<T>::moveStack(ArrayStack<T>&& other) noexcept {
    delete[] data;
    data = other.data;
    topIdx = other.topIdx;
    capacity = other.capacity;
    other.data = nullptr;
    other.topIdx = 0;
    other.capacity = 0;
}

template<typename T>
bool ArrayStack<T>::isFull() const {
    return topIdx == capacity;

}

template<typename T>
bool ArrayStack<T>::isEmpty() const {
    return topIdx == 0;
}

template<typename T>
void ArrayStack<T>::push(const T& item) {
    assert(!isFull());
    data[topIdx++] = item;
}

template<typename T>
void ArrayStack<T>::push(T&& item) {
    assert(!isFull());
    data[topIdx++] = std::move(item);
}

template<typename T>
void ArrayStack<T>::pop() {
    assert(!isEmpty());
    topIdx--;
}

template<typename T>
T ArrayStack<T>::top() const {
    assert(!isEmpty());
    return data[topIdx - 1];
}

template<typename T>
long long ArrayStack<T>::size() const {
    return topIdx;
}

template<typename T>
void ArrayStack<T>::print() const {
    if (isEmpty()) {
        std::cout << "Stack is empty.\n";
        return;
    }
    long long topindex = topIdx;
    while (topindex > 0) {
        std::cout << data[topindex - 1] << " ";
        topindex--;
    }
    std::cout << "\n";
}

template<typename T>
void ArrayStack<T>::initializeStack() {
    topIdx = 0;
}