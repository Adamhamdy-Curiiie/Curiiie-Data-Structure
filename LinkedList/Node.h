#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <iostream>

template <typename T>
class Node {
public:
    T data;
    Node* next;

    Node(T val) : data(val), next(nullptr) {}

    ~Node() {
        std::cout << "Destroyed node: " << data << " at " << this << "\n";
    }
};

template <typename T>
class LinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int length;

    std::vector<Node<T>*> debug_data;

    void debug_track_add(Node<T>* node) {
        debug_data.push_back(node);
    }

    void debug_track_remove(Node<T>* node) {
        auto it = std::find(debug_data.begin(), debug_data.end(), node);
        if (it == debug_data.end())
            std::cout << "Node not found in debug tracker\n";
        else
            debug_data.erase(it);
    }

public:
    LinkedList();
    LinkedList(T val);
    ~LinkedList();

    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;

    // Insertion
    void push_back(const T& val);
    void push_front(const T& val);

    // Deletion
    void pop_front();
    void pop_back();
    void remove_at(int index);
    void remove_value(int val);

    // Access
    Node<T>* get_nth_from_front(T n) const;
    Node<T>* get_nth_from_back(T n) const;

    // Search
    Node<T>* find_by_value(T val) const;
    T        find_index_of(int val) const;
    int      transposition_search(T val);

    // Utilities
    void swap_pairs();
    void print() const;
    bool equals(LinkedList<T>& other) const;
    bool equals(LinkedList<T>& other, int other_length) const;

    int  size()  const { return length; }
    bool empty() const { return length == 0; }

    // Debug
    void        debug_print_addresses()  const;
    void        debug_print_node(Node<T>* node, bool is_separate = false) const;
    void        debug_print_list(std::string msg = "")  const;
    std::string debug_to_string()        const;
    void        debug_verify_integrity() const;
};

#include "Node.cpp"
#endif // LINKED_LIST_H
