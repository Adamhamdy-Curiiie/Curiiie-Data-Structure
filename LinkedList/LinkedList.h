#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "LinkedListBase.h"

// ═══════════════════════════════════════════════════════════════════════════════
// ─── LinkedList — unordered singly linked list ────────────────────────────────
// ═══════════════════════════════════════════════════════════════════════════════
//
// Inherits the common interface and debug tracker from LinkedListBase<T>.
// Adds its own head / tail pointers (SNode<T>*) and implements every virtual
// method.  Operations that only make sense for an unordered list — such as
// transposition_search, divide_at, rotate — are concrete non-virtual members.
//
// ═══════════════════════════════════════════════════════════════════════════════

template <typename T>
class LinkedList : public LinkedListBase<T> {

    // Bring inherited members into scope (required for templates).
    using LinkedListBase<T>::length;
    using LinkedListBase<T>::debug_data;
    using LinkedListBase<T>::debug_track_add;
    using LinkedListBase<T>::debug_track_remove;

private:
    SNode<T>* head;
    SNode<T>* tail;

    // ── Private helpers ──────────────────────────────────────────────────────

    // Recursive helper for reverse_print. Space: O(n) call stack.
    void reverse_print_helper(SNode<T>* curr) const {
        if (curr == nullptr) return;
        reverse_print_helper(curr->next);
        std::cout << curr->data << " ";
    }

    // Deep-copy the node chain from other into this list.
    // Used by the copy constructor and copy_list().
    void copy_from(const LinkedList<T>& other);

public:
    // ── Constructors / Destructor ─────────────────────────────────────────────
    LinkedList();
    explicit LinkedList(T val);
    ~LinkedList() override;

    // Deep copy constructor — independent node-by-node duplicate.
    // Time: O(n).
    LinkedList(const LinkedList<T>& other);

    // Assignment operator — destroy then copy.  Guards against self-copy.
    // Time: O(n).
    LinkedList<T>& operator=(const LinkedList<T>& other);

    // ── Core ADT (override LinkedListBase pure-virtuals) ──────────────────────

    // Returns true when head == nullptr.  Time: O(1).
    bool is_empty() const override { return head == nullptr; }

    // Free every node; reset head / tail / length.
    // Book: destroyList (p.287).  Time: O(n).
    void destroy() override;

    // Alias for destroy().  Book: initializeList (p.287).  Time: O(n).
    void initialize() override { destroy(); }

    // Return data of the first node.  Asserts non-empty.  Time: O(1).
    T front() const override;

    // Return data of the last node.  Asserts non-empty.  Time: O(1).
    T back() const override;

    // Node-for-node copy from other.
    // Book: copyList (p.289).  Time: O(n).
    void copy_list(const LinkedList<T>& other);

    // ── Iterator support (begin / end for range-based for) ───────────────────
    SinglyIterator<T> begin() const { return SinglyIterator<T>(head); }
    SinglyIterator<T> end()   const { return SinglyIterator<T>(nullptr); }

    // ── Print ─────────────────────────────────────────────────────────────────

    // Forward print — elements separated by spaces.  Time: O(n).
    void print() const override;

    // Reverse print via recursion.
    // Book: reversePrint (Ch.5 p.364).  Time: O(n).  Space: O(n).
    void reverse_print() const override;

    // ── Search ────────────────────────────────────────────────────────────────

    // Sequential search — true if val is in the list.
    // Book: search() on unorderedLinkedList (p.293).
    // Time: O(1) best, O(n) worst.
    bool search(const T& val) const override;

    // Pointer to first node whose data == val, or nullptr.
    // Time: O(1) best, O(n) worst.
    SNode<T>* find_by_value(T val) const;

    // 0-based index of first match, or -1.
    // Time: O(1) best, O(n) worst.
    int find_index_of(T val) const;

    // Swap found node with its predecessor to speed up future lookups.
    // Book: transposition heuristic (Ch.9).
    // Time: O(1) best, O(n) worst.
    int transposition_search(T val);

    // ── Insertion ─────────────────────────────────────────────────────────────

    // Append at tail.  Book: insertLast (p.294).  Time: O(1).
    void push_back(const T& val) override;

    // Prepend at head.  Book: insertFirst (p.294).  Time: O(1).
    void push_front(const T& val) override;

    // Insert at 1-based position.
    // Time: O(1) best (index == 1), O(n) worst.
    void insert_at(int index, const T& val) override;

    // ── Deletion ──────────────────────────────────────────────────────────────

    // Remove head.  Time: O(1).
    void pop_front() override;

    // Remove tail.  Time: O(n) — must walk to predecessor.
    void pop_back() override;

    // Remove node at 1-based index.
    // Time: O(1) best (index == 1), O(n) worst.
    void remove_at(int index) override;

    // Remove first node whose data == val.
    // Book: deleteNode (p.296-298).  Time: O(1) best, O(n) worst.
    void remove_value(const T& val) override;

    // Remove the smallest element.
    // Book: Programming Exercise 2, Ch.5.  Time: O(n).
    void remove_min() override;

    // Remove every occurrence of val in one pass.
    // Book: Programming Exercise 2, Ch.5.  Time: O(n).
    void remove_all_occurrences(const T& val) override;

    // ── Access ────────────────────────────────────────────────────────────────

    // Pointer to n-th node (1-based).  Time: O(1) best, O(n) worst.
    SNode<T>* get_nth_from_front(int n) const;

    // Pointer to n-th node from the back (1-based).  Time: O(n).
    SNode<T>* get_nth_from_back(int n) const;

    // Data at 1-based position k.  Asserts in range.  Time: O(n).
    // Book: Programming Exercise 3a, Ch.5.
    T get_kth_element(int k) const override;

    // Delete the k-th node (1-based).  Time: O(n).
    // Book: Programming Exercise 3b, Ch.5.
    void delete_kth_element(int k) override;

    // ── Equality ─────────────────────────────────────────────────────────────

    bool operator==(const LinkedList<T>& other) const;

    // ── Utilities ─────────────────────────────────────────────────────────────

    // Swap adjacent pairs of data values in-place.  Time: O(n).
    void swap_pairs() override;

    // Split into two halves using slow/fast pointer (tortoise-and-hare).
    // "this" keeps the first half; sublist receives the second half.
    // Book: divideMid, Programming Exercise 4 (p.349).  Time: O(n).
    void divide_mid(LinkedList<T>& sublist);

    // Split at the first node whose data == item.
    // "this" keeps nodes before item; sublist starts at item.
    // Book: divideAt, Programming Exercise 5 (p.350).  Time: O(n).
    void divide_at(LinkedList<T>& sublist, const T& item);

    // Move head to tail.
    // Book: rotate(), Programming Exercise 17 (p.353).  Time: O(1).
    void rotate() override;

    // In-place reversal.  Time: O(n), Space: O(1).
    void reverse() override;

    // ── Debug ─────────────────────────────────────────────────────────────────
    void        debug_print_addresses()  const;
    void        debug_print_node(SNode<T>* node, bool is_separate = false) const;
    void        debug_print_list(std::string msg = "") const;
    std::string debug_to_string()        const override;
    void        debug_verify_integrity() const override;
};

#include "LinkedList.cpp"
#endif // LINKED_LIST_H
