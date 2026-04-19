#include "LinkedStack.h"
#include <iostream>
#include <cassert>

template <typename T>
LinkedStack<T>::LinkedStack() : topNode(nullptr), length(0) {}

template <typename T>
LinkedStack<T>::LinkedStack(const LinkedStack<T>& other)
    : topNode(nullptr), length(0) {
    copyStack(other);
}

template <typename T>
LinkedStack<T>& LinkedStack<T>::operator=(const LinkedStack<T>& other) {
    if (this != &other) {
        initializeStack();   // free existing nodes first
        copyStack(other);
    }
    return *this;
}

template <typename T>
LinkedStack<T>::~LinkedStack() {
    initializeStack();
}


template <typename T>
void LinkedStack<T>::initializeStack() {
    StackNode<T>* temp;

    while (!isEmpty()) {
        temp = topNode;
        topNode = topNode->next;
        delete temp;
    }
    length = 0;
}

template <typename T>
bool LinkedStack<T>::isFull() const {
    return false;   // heap never full
}

template <typename T>
bool LinkedStack<T>::isEmpty() const {
    return topNode == nullptr;
}

template <typename T>
void LinkedStack<T>::push(const T& element) {
    StackNode<T>* newNode;
    newNode = new StackNode<T>(element);
    newNode->next = topNode;
    topNode = newNode;
    length++;
}

template <typename T>
void LinkedStack<T>::pop() {
    assert(topNode != nullptr);
    StackNode<T>* temp = topNode;
    topNode = topNode->next;
    delete temp;
    length--;
}

template <typename T>
T LinkedStack<T>::top() const {
    // TODO — assert not empty, return topNode->data
    assert(topNode != nullptr);
    return topNode->data;
}

template <typename T>
int LinkedStack<T>::size() const {
    return length;
}

template <typename T>
void LinkedStack<T>::print() const {
    // TODO — walk from topNode, print each node's data
    StackNode<T>* temp = topNode;
    while (temp != nullptr) {
        std::cout << temp->data << '\n';
        temp = temp->next;
    }
}


template <typename T>
void LinkedStack<T>::copyStack(const LinkedStack<T>& other) {
    StackNode<T>* newNode, *current, *lastNode;
    if (!isEmpty()) {
        initializeStack();
    }
    if (other.topNode == nullptr) {
        topNode = nullptr;
    }else {
        current = other.topNode;

        topNode = new StackNode<T>;
        topNode->data = current->data;
        topNode->next = nullptr;
        lastNode = topNode;

        current = current->next;
        while (current != nullptr) {
            newNode = new StackNode<T>;
            newNode->data = current->data;
            newNode->next = nullptr;
            lastNode->next = newNode;
            lastNode = newNode;
            current = current->next;
        }
    }
    length = other.length;
}