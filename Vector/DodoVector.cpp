#include <iostream>
#include "DodoVector.h"
using namespace std;

// ─── Constructors / Destructor ────────────────────────────────────────────────

template <typename T>
Vector<T>::Vector(int capacity) : size(0), capacity(capacity) {
    data = new T[capacity];
    cout << "Vector constructor called\n";
}

template <typename T>
Vector<T>::Vector(initializer_list<T> list) {
    size     = list.size();
    capacity = list.size() + 2;
    data     = new T[capacity];

    int i = 0;
    for (auto& element : list)
        data[i++] = element;
}

template <typename T>
Vector<T>::Vector(const Vector<T>& other) {
    capacity = other.capacity;
    size     = other.size;
    data     = new T[capacity];

    for (int i = 0; i < size; i++)
        data[i] = other.data[i];
}

template <typename T>
Vector<T>::~Vector() {
    delete[] data;
    cout << "\nVector destructor called\n";
}

// ─── Operators ────────────────────────────────────────────────────────────────

template <typename T>
T& Vector<T>::operator[](int index) {
    if (index < 0 || index >= size) {
        cout << "Index out of range.\n";
        exit(-1);
    }
    return data[index];
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
    if (this != &other) {
        delete[] data;
        capacity = other.capacity;
        size     = other.size;
        data     = new T[capacity];

        for (int i = 0; i < size; i++)
            data[i] = other.data[i];
    }
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept {
    if (this != &other) {
        delete[] data;
        data     = other.data;
        capacity = other.capacity;
        size     = other.size;

        other.data     = nullptr;
        other.size     = 0;
        other.capacity = 0;
    }
    return *this;
}

// ─── Private ──────────────────────────────────────────────────────────────────

template <typename T>
void Vector<T>::expand_capacity() {
    capacity *= 2;
    T* new_data = new T[capacity];

    for (int i = 0; i < size; i++)
        new_data[i] = data[i];

    delete[] data;
    data = new_data;
}

// ─── Insertion / Deletion ─────────────────────────────────────────────────────

template <typename T>
void Vector<T>::push_back(T element) {
    if (size >= capacity)
        expand_capacity();
    data[size++] = element;
}

template <typename T>
void Vector<T>::pop_back() {
    if (size > 0)
        size--;
}

template <typename T>
void Vector<T>::assign(int index, T value) {
    if (index < 0 || index >= size) {
        cout << "Index out of range.\n";
        exit(-1);
    }
    data[index] = value;
}

template <typename T>
void Vector<T>::insert(int index, T element) {
    if (index < 0 || index >= size) {
        cout << "Index out of range.\n";
        exit(-1);
    }
    if (size >= capacity)
        expand_capacity();

    for (int i = size - 1; i >= index; i--)
        data[i + 1] = data[i];

    data[index] = element;
    size++;
}

template <typename T>
void Vector<T>::delete_at(int index) {
    for (int i = index; i < size - 1; i++)
        data[i] = data[i + 1];
    size--;
}

// ─── Rotation ─────────────────────────────────────────────────────────────────

template <typename T>
void Vector<T>::rotate_right() {
    T last = data[size - 1];
    for (int i = size - 1; i > 0; i--)
        data[i] = data[i - 1];
    data[0] = last;
}

template <typename T>
void Vector<T>::rotate_right(int times) {
    times %= size;
    for (int i = 0; i < times; i++)
        rotate_right();
}

template <typename T>
void Vector<T>::rotate_left() {
    T first = data[0];
    for (int i = 0; i < size - 1; i++)
        data[i] = data[i + 1];
    data[size - 1] = first;
}

template <typename T>
void Vector<T>::rotate_left(int times) {
    times %= size;
    for (int i = 0; i < times; i++)
        rotate_left();
}

// ─── Search ───────────────────────────────────────────────────────────────────

template <typename T>
int Vector<T>::transposition_find(int value) {
    for (int i = 0; i < size; i++) {
        if (data[i] == value) {
            if (i == 0) return 0;
            swap(data[i - 1], data[i]);
            return i - 1;
        }
    }
    return -1;
}
