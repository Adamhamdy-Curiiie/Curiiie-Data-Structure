#ifndef VECTOR_H
#define VECTOR_H

#include <initializer_list>

template <typename T>
class Vector {
private:
    int size;
    int capacity;
    T*  data;

    void expand_capacity();

public:
    Vector(int capacity = 2);
    Vector(std::initializer_list<T> list);
    Vector(const Vector<T>& other);
    ~Vector();

    T&       operator[](int index);
    Vector<T>& operator=(const Vector<T>& other);
    Vector<T>& operator=(Vector<T>&& other) noexcept;

    int  get_size()     const { return size; }
    int  get_capacity() const { return capacity; }

    void push_back(T element);
    void pop_back();
    void assign(int index, T value);
    void insert(int index, T element);
    void delete_at(int index);

    void rotate_right();
    void rotate_right(int times);
    void rotate_left();
    void rotate_left(int times);

    int  transposition_find(int value);
};

#include "DodoVector.cpp"
#endif // VECTOR_H
