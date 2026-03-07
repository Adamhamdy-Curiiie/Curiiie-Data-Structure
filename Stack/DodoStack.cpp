#include <iostream>
#include "DodoStack.h"
using namespace std;

// ─── Constructors / Destructor ────────────────────────────────────────────────

template <typename T>
Stack<T>::Stack(int capacity) : size(0), capacity(capacity) {
    data = new T[capacity];
    cout << "Stack constructor called\n";
}

template <typename T>
Stack<T>::~Stack() {
    delete[] data;
    cout << "Stack destructor called\n";
}

// ─── Private ──────────────────────────────────────────────────────────────────

template <typename T>
void Stack<T>::expand_capacity() {
    capacity *= 2;
    T* new_data = new T[capacity];

    for (int i = 0; i < size; i++)
        new_data[i] = data[i];

    delete[] data;
    data = new_data;
    cout << "Capacity expanded to " << capacity << "\n";
}

// ─── Public ───────────────────────────────────────────────────────────────────

template <typename T>
void Stack<T>::push(T element) {
    if (size >= capacity)
        expand_capacity();
    data[size++] = element;
}

template <typename T>
void Stack<T>::pop() {
    if (size > 0)
        size--;
    else
        cout << "Stack is empty! Cannot pop.\n";
}

template <typename T>
T Stack<T>::top() const {
    if (size > 0)
        return data[size - 1];

    cout << "Stack is empty!\n";
    exit(-1);
}
