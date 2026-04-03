#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include "LinkedListBase.h"

// ═══════════════════════════════════════════════════════════════════════════════
// ─── DoublyLinkedList — doubly linked list ────────────────────────────────────
// ═══════════════════════════════════════════════════════════════════════════════
//
// Inherits the common interface and debug tracker from LinkedListBase<T>.
// Each node carries both next and prev pointers, giving O(1) pop_back and
// O(n/2) traversal for positional operations.
// ═══════════════════════════════════════════════════════════════════════════════

template <typename T>
class DoublyLinkedList : public LinkedListBase<T> {

    using LinkedListBase<T>::length;
    using LinkedListBase<T>::debug_data;
    using LinkedListBase<T>::debug_track_add;
    using LinkedListBase<T>::debug_track_remove;

private:
    DNode<T>* head;
    DNode<T>* tail;

    // ── Private helpers ──────────────────────────────────────────────────────
    void reverse_print_helper(DNode<T>* curr) const {
        if (curr == nullptr) return;
        reverse_print_helper(curr->next);
        std::cout << curr->data << " ";
    }

    void copy_from(const DoublyLinkedList<T>& other);

public:
    // ── Constructors / Destructor ─────────────────────────────────────────────
    DoublyLinkedList();
    explicit DoublyLinkedList(T val);
    ~DoublyLinkedList() override;

    DoublyLinkedList(const DoublyLinkedList<T>& other);
    DoublyLinkedList<T>& operator=(const DoublyLinkedList<T>& other);

    // ── Core ADT ──────────────────────────────────────────────────────────────
    bool is_empty() const override { return head == nullptr; }

    // Free every node; reset head / tail / length.  Time: O(n).
    void destroy() override;
    void initialize() override { destroy(); }

    // Return data of the first node.  Asserts non-empty.  Time: O(1).
    T front() const override;

    // Return data of the last node.  Asserts non-empty.  Time: O(1).
    T back() const override;

    void copy_list(const DoublyLinkedList<T>& other);

    // ── Iterator support ──────────────────────────────────────────────────────
    DoublyIterator<T> begin()  const { return DoublyIterator<T>(head); }
    DoublyIterator<T> end()    const { return DoublyIterator<T>(nullptr); }
    DoublyIterator<T> rbegin() const { return DoublyIterator<T>(tail); }
    DoublyIterator<T> rend()   const { return DoublyIterator<T>(nullptr); }

    // ── Print ─────────────────────────────────────────────────────────────────

    // Forward print.  Time: O(n).
    void print() const override;

    // Iterative backward print via prev — no recursion or stack needed.
    // Time: O(n).  Space: O(1).
    void print_backward() const;

    // Recursive backward print (mirrors singly linked API).
    // Time: O(n).  Space: O(n).
    void reverse_print() const override;

    // ── Search ────────────────────────────────────────────────────────────────

    // Sequential search.  Time: O(1) best, O(n) worst.
    bool search(const T& val) const override;

    // Pointer to first matching node, or nullptr.  Time: O(1) best, O(n) worst.
    DNode<T>* find_by_value(T val) const;

    // 0-based index of first match, or -1.  Time: O(1) best, O(n) worst.
    int find_index_of(T val) const;

    // Swap found node with predecessor to bias future lookups toward head.
    // In a doubly linked list the swap touches both nodes' prev links.
    // Book: transposition heuristic (Ch.9).
    // Time: O(1) best, O(n) worst.
    int transposition_search(T val);

    // ── Insertion ─────────────────────────────────────────────────────────────

    // Append at tail.  Time: O(1).
    void push_back(const T& val) override;

    // Prepend at head.  Time: O(1).
    void push_front(const T& val) override;

    // Insert at 1-based position, walking from the closer end.
    // Time: O(1) best (index == 1 or index == length+1), O(n/2) avg.
    void insert_at(int index, const T& val) override;

    // ── Deletion ──────────────────────────────────────────────────────────────

    // Remove head.  Time: O(1).
    void pop_front() override;

    // Remove tail.  Time: O(1) — key advantage over singly linked.
    void pop_back() override;

    // Remove node at 1-based index, walking from the closer end.
    // Time: O(1) best (index == 1 or index == length), O(n/2) avg.
    void remove_at(int index) override;

    // Remove first node whose data == val.
    // Time: O(1) best, O(n) worst.
    void remove_value(const T& val) override;

    // Remove the smallest element.  Time: O(n).
    void remove_min() override;

    // Remove every occurrence of val.  Time: O(n).
    void remove_all_occurrences(const T& val) override;

    // ── Access ────────────────────────────────────────────────────────────────

    // Pointer to n-th node (1-based) from the front — walks from closer end.
    // Time: O(1) best (n == 1 or n == length), O(n/2) avg.
    DNode<T>* get_nth_from_front(int n) const;

    // Pointer to n-th node from the back (1-based) — walks backward via prev.
    // Time: O(1) best (n == 1), O(n) worst.
    DNode<T>* get_nth_from_back(int n) const;

    // Data at 1-based position k.  Terminates if out of range.  Time: O(n).
    T get_kth_element(int k) const override;

    // Delete the k-th node (1-based).  Time: O(n).
    void delete_kth_element(int k) override;

    // ── Equality ─────────────────────────────────────────────────────────────
    bool equals(const DoublyLinkedList<T>& other) const;
    bool operator==(const DoublyLinkedList<T>& other) const;

    // ── Utilities ─────────────────────────────────────────────────────────────

    // Swap adjacent pairs of data values in-place.  Time: O(n).
    void swap_pairs() override;

    // Split into two nearly-equal halves (tortoise-and-hare).
    // Correctly zeroes the prev pointer of the new sublist head.
    // Time: O(n).
    void divide_mid(DoublyLinkedList<T>& sublist);

    // Split at the first node whose data == item.
    // "this" keeps nodes before item; sublist starts at item.
    // Time: O(n).
    void divide_at(DoublyLinkedList<T>& sublist, const T& item);

    // Move head to tail.  Time: O(1).
    void rotate() override;

    // Move tail to head — O(1) only possible because of prev pointer.
    void rotate_backward();

    // In-place reversal by swapping next/prev on every node.
    // Time: O(n), Space: O(1).
    void reverse() override;

    // ── Debug ─────────────────────────────────────────────────────────────────
    void        debug_print_addresses()  const;
    void        debug_print_node(DNode<T>* node, bool is_separate = false) const;
    void        debug_print_list(std::string msg = "") const;
    std::string debug_to_string()        const override;
    void        debug_verify_integrity() const override;
};

#include "DoublyLinkedList.cpp"
#endif
