#ifndef LINKED_LIST_BASE_H
#define LINKED_LIST_BASE_H

#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <iostream>
#include <cassert>

// ─── SNode — Singly linked node (used by LinkedList and OrderedLinkedList) ────
template <typename T>
struct SNode {
    T      data;
    SNode* next;

    SNode(T val) : data(val), next(nullptr) {}
    ~SNode() {
        std::cout << "Destroyed node: " << data << " at " << this << "\n";
    }
};

// ─── DNode — Doubly linked node (used by DoublyLinkedList) ───────────────────
template <typename T>
struct DNode {
    T      data;
    DNode* next;
    DNode* prev;

    DNode(T val) : data(val), next(nullptr), prev(nullptr) {}
    ~DNode() {
        std::cout << "Destroyed node: " << data << " at " << this << "\n";
    }
};


// ═══════════════════════════════════════════════════════════════════════════════
// ─── Iterators ────────────────────────────────────────────────────────────────
// ═══════════════════════════════════════════════════════════════════════════════

// ─── SinglyIterator — forward-only iterator over SNode<T> chains ──────────────
// Time: every operation is O(1).
template <typename T>
class SinglyIterator {
public:
    SinglyIterator() : current(nullptr) {}
    explicit SinglyIterator(SNode<T>* ptr) : current(ptr)       {}

    T operator*()  const { return current->data; }

    SinglyIterator<T>& operator++() { current = current->next; return *this; }

    bool operator==(const SinglyIterator<T>& o) const { return current == o.current; }
    bool operator!=(const SinglyIterator<T>& o) const { return current != o.current; }

private:
    SNode<T>* current;
};

// ─── DoublyIterator — bidirectional iterator over DNode<T> chains ─────────────
// Time: every operation is O(1).
template <typename T>
class DoublyIterator {
public:
    DoublyIterator() : current(nullptr) {}
    explicit DoublyIterator(DNode<T>* ptr) : current(ptr) {}

    T operator*() const { return current->data; }

    DoublyIterator<T>& operator++() { current = current->next; return *this; }
    DoublyIterator<T>& operator--() { current = current->prev; return *this; }

    bool operator==(const DoublyIterator<T>& o) const { return current == o.current; }
    bool operator!=(const DoublyIterator<T>& o) const { return current != o.current; }

private:
    DNode<T>* current;
};


// ═══════════════════════════════════════════════════════════════════════════════
// ─── LinkedListBase — abstract base for all three list variants ───────────────
// ═══════════════════════════════════════════════════════════════════════════════
//
// Holds only the state that is common to every list: length and the debug
// tracker (a flat vector of raw pointers whose concrete type is erased via
// void*).  All node-type-specific work lives in the derived classes.
//
// Pure-virtual interface mirrors the common API shared across all three lists
// so that a pointer/reference to LinkedListBase<T> can call any of them.
// ═══════════════════════════════════════════════════════════════════════════════

template <typename T>
class LinkedListBase {
protected:
    int length;

    // ── Debug tracker (type-erased, works for SNode* and DNode*) ────────────
    std::vector<void*> debug_data;

    void debug_track_add(void* node)    { debug_data.push_back(node); }
    void debug_track_remove(void* node) {
        auto it = std::find(debug_data.begin(), debug_data.end(), node);
        if (it == debug_data.end())
            std::cout << "Node not found in debug tracker\n";
        else
            debug_data.erase(it);
    }

public:
    LinkedListBase()          : length(0) {}
    virtual ~LinkedListBase() = default;

    int  size()  const { return length; }
    bool empty() const { return length == 0; }

    virtual bool is_empty()                   const = 0;
    virtual void destroy()                          = 0;
    virtual void initialize()                       = 0;
    virtual T    front()                      const = 0;
    virtual T    back()                       const = 0;
    virtual void print()                      const = 0;
    virtual void reverse_print()              const = 0;
    virtual bool search(const T& val)         const = 0;
    virtual void push_back(const T& val)            = 0;
    virtual void push_front(const T& val)           = 0;
    virtual void insert_at(int idx, const T& val)   = 0;
    virtual void pop_front()                        = 0;
    virtual void pop_back()                         = 0;
    virtual void remove_at(int idx)                 = 0;
    virtual void remove_value(const T& val)         = 0;
    virtual void remove_min()                       = 0;
    virtual void remove_all_occurrences(const T& v) = 0;
    virtual T    get_kth_element(int k)       const = 0;
    virtual void delete_kth_element(int k)          = 0;
    virtual void swap_pairs()                       = 0;
    virtual void rotate()                           = 0;
    virtual void reverse()                          = 0;
    virtual std::string debug_to_string()     const = 0;
    virtual void debug_verify_integrity()     const = 0;
};

#endif
