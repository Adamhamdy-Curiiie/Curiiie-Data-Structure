#ifndef ORDERED_LINKED_LIST_H
#define ORDERED_LINKED_LIST_H

#include "LinkedListBase.h"

// ═══════════════════════════════════════════════════════════════════════════════
// ─── OrderedLinkedList — sorted singly linked list ────────────────────────────
// ═══════════════════════════════════════════════════════════════════════════════
//
// Inherits the common interface and debug tracker from LinkedListBase<T>.
// Elements are always kept in non-decreasing order.  Because order is enforced,
// insertion is always O(n), but min/max are O(1) and search can exit early.
//
// Requires T to support operator< and operator==.
// ═══════════════════════════════════════════════════════════════════════════════

template <typename T>
class OrderedLinkedList : public LinkedListBase<T> {

    using LinkedListBase<T>::length;
    using LinkedListBase<T>::debug_data;
    using LinkedListBase<T>::debug_track_add;
    using LinkedListBase<T>::debug_track_remove;

private:
    SNode<T>* head;
    SNode<T>* tail;

    // ── Private helpers ──────────────────────────────────────────────────────
    void reverse_print_helper(SNode<T>* curr) const {
        if (curr == nullptr) return;
        reverse_print_helper(curr->next);
        std::cout << curr->data << " ";
    }

    void copy_from(const OrderedLinkedList<T>& other);

public:
    // ── Constructors / Destructor ─────────────────────────────────────────────
    OrderedLinkedList();
    explicit OrderedLinkedList(T val);
    ~OrderedLinkedList() override;

    OrderedLinkedList(const OrderedLinkedList<T>& other);
    OrderedLinkedList<T>& operator=(const OrderedLinkedList<T>& other);

    // ── Core ADT ──────────────────────────────────────────────────────────────
    bool is_empty() const override { return head == nullptr; }

    // Free every node; reset head / tail / length.  Time: O(n).
    void destroy() override;
    void initialize() override { destroy(); }

    // Return the minimum element (head).  Time: O(1).
    T front() const override;

    // Return the maximum element (tail).  Time: O(1).
    T back() const override;

    void copy_list(const OrderedLinkedList<T>& other);

    // Aliases for the sorted list (min == front, max == back).
    T min() const { return front(); }
    T max() const { return back();  }

    // ── Iterator support ──────────────────────────────────────────────────────
    SinglyIterator<T> begin() const { return SinglyIterator<T>(head); }
    SinglyIterator<T> end()   const { return SinglyIterator<T>(nullptr); }

    // ── Print ─────────────────────────────────────────────────────────────────

    // Forward print — ascending order.  Time: O(n).
    void print() const override;

    // Recursive backward print — descending order.
    // Time: O(n).  Space: O(n).
    void reverse_print() const override;

    // ── Search ────────────────────────────────────────────────────────────────

    // Sequential search with early exit (stops when node->data > val).
    // Book: search() on orderedLinkedList (Ch.5 pp.301-302).
    // Time: O(1) best, O(n) worst.
    bool search(const T& val) const override;

    // Pointer to first node whose data == val, or nullptr.  Early exit.
    // Time: O(1) best, O(n) worst.
    SNode<T>* find_by_value(T val) const;

    // 0-based index of first match, or -1.  Early exit.
    // Time: O(1) best, O(n) worst.
    int find_index_of(T val) const;

    // Conceptual binary search — still O(n) on a linked list.
    // Time: O(n).
    bool binary_search(const T& val) const;

    // ── Insertion ─────────────────────────────────────────────────────────────

    // Insert in sorted position — the ONLY insertion method.
    // Book: insert() on orderedLinkedList (Ch.5 pp.303-307).
    // Time: O(1) best (val is smallest), O(n) worst (val is largest).
    void insert(const T& val);

    // Convenience aliases — all route to insert() to preserve order.
    // NOTE: the index argument of insert_at is ignored.
    void push_back(const T& val)             override { insert(val); }
    void push_front(const T& val)            override { insert(val); }
    void insert_at(int /*idx*/, const T& val) override { insert(val); }

    // ── Deletion ──────────────────────────────────────────────────────────────

    // Remove the minimum (head).  Time: O(1).
    void pop_front() override;

    // Remove the maximum (tail).  Time: O(n) — singly linked.
    void pop_back() override;

    // Remove node at 1-based position.
    // Time: O(1) best (index == 1), O(n) worst.
    void remove_at(int index) override;

    // Remove first node whose data == val.  Early exit.
    // Time: O(1) best, O(n) worst.
    void remove_value(const T& val) override;

    // Remove minimum (head).  O(1) — always the first node.
    void remove_min() override { pop_front(); }

    // Remove maximum (tail).  O(n) — need predecessor.
    void remove_max()          { pop_back(); }

    // Remove every occurrence of val.  Early exit.  Time: O(n).
    void remove_all_occurrences(const T& val) override;

    // ── Merge ─────────────────────────────────────────────────────────────────

    // Merge two sorted lists into one sorted list in O(m + n).
    // other is left empty after the call.
    // Book: mergeList() (Ch.5 Programming Exercise).
    void merge(OrderedLinkedList<T>& other);

    // ── Access ────────────────────────────────────────────────────────────────
    SNode<T>* get_nth_from_front(int n) const;
    SNode<T>* get_nth_from_back(int n)  const;

    // Data at 1-based position k.  Terminates if out of range.  Time: O(n).
    T get_kth_element(int k) const override;

    // Delete the k-th node (1-based).  Time: O(n).
    void delete_kth_element(int k) override;

    // ── Equality ─────────────────────────────────────────────────────────────
    bool equals(const OrderedLinkedList<T>& other) const;
    bool operator==(const OrderedLinkedList<T>& other) const;

    // ── Utilities ─────────────────────────────────────────────────────────────

    // Split into two sorted halves (tortoise-and-hare).
    // Book: divideMid, Ch.5 Programming Exercise 4 (p.349).  Time: O(n).
    void divide_mid(OrderedLinkedList<T>& sublist);

    // Count occurrences of val.  Early exit.  Time: O(k).
    int count(const T& val) const;

    // True if elements are in non-decreasing order (should always be true).
    // Time: O(n).
    bool is_sorted() const;

    // swap_pairs — swaps data values in adjacent pairs.  Time: O(n).
    // NOTE: may temporarily violate the sorted invariant; use with care.
    void swap_pairs() override;

    // rotate — move head to tail.  Time: O(1).
    // NOTE: may violate sorted invariant on a non-trivial list; use with care.
    void rotate() override;

    // In-place reversal — after this call the list is in DESCENDING order.
    // WARNING: violates the sorted invariant.  Time: O(n), Space: O(1).
    void reverse() override;

    // ── Debug ─────────────────────────────────────────────────────────────────
    void        debug_print_addresses()  const;
    void        debug_print_node(SNode<T>* node, bool is_separate = false) const;
    void        debug_print_list(std::string msg = "") const;
    std::string debug_to_string()        const override;
    void        debug_verify_integrity() const override;
};

#include "OrderedLinkedList.cpp"
#endif