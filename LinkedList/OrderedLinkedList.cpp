#include <iostream>
#include <cassert>
#include "OrderedLinkedList.h"
using namespace std;

//  TIME COMPLEXITY — Ordered (Sorted) Singly Linked List
//
//  Operation           Best    Avg     Worst   Ordered         Unordered
// ─────────────────────────────────────────────────────────────────────────────
//  insert(v)           O(1)    O(n)    O(n)    O(n)            O(1)
//    → ordered: must walk to find the correct sorted position
//    → unordered: always push_front or push_back, both O(1)
//  pop_front/min       O(1)    O(1)    O(1)    O(1) ← WIN      O(n) unordered min
//  pop_back/max        O(n)    O(n)    O(n)    O(n)            O(n)
//  search(v)           O(1)    O(n)    O(n)    early exit ← faster O(n)
//  min() / max()       O(1)    O(1)    O(1)    O(1) ← WIN      O(n)
//  merge(other)        O(n+m)  O(n+m)  O(n+m)  O(n+m) ← WIN   O(n·m)


// ─── Constructors / Destructor ────────────────────────────────────────────────

template <typename T>
OrderedLinkedList<T>::OrderedLinkedList()
    : LinkedListBase<T>(), head(nullptr), tail(nullptr) {}

template <typename T>
OrderedLinkedList<T>::OrderedLinkedList(T val)
    : LinkedListBase<T>(), head(new SNode<T>(val)), tail(nullptr) {
    tail   = head;
    length = 1;
    debug_track_add(head);
}

template <typename T>
OrderedLinkedList<T>::OrderedLinkedList(const OrderedLinkedList<T>& other)
    : LinkedListBase<T>(), head(nullptr), tail(nullptr) {
    copy_list(other);
}

template <typename T>
OrderedLinkedList<T>& OrderedLinkedList<T>::operator=(const OrderedLinkedList<T>& other) {
    if (this != &other) { destroy(); copy_list(other); }
    return *this;
}

template <typename T>
OrderedLinkedList<T>::~OrderedLinkedList() {
    SNode<T>* curr = head;
    while (curr != nullptr) {
        SNode<T>* next = curr->next;
        delete curr;
        curr = next;
    }
}


// ─── Core ADT ─────────────────────────────────────────────────────────────────

// destroy — free every node; reset head / tail / length.  Time: O(n).
template <typename T>
void OrderedLinkedList<T>::destroy() {
    SNode<T>* temp;
    while (head != nullptr) {
        temp = head;
        head = head->next;
        debug_track_remove(temp);
        delete temp;
    }
    tail   = nullptr;
    length = 0;
}

// copy_list — node-for-node copy from other into this list.
// Source is already sorted, so the copy is sorted too.  Time: O(n).
template <typename T>
void OrderedLinkedList<T>::copy_list(const OrderedLinkedList<T>& other) {
    if (head != nullptr) destroy();
    if (other.head == nullptr) { head = tail = nullptr; length = 0; return; }

    SNode<T>* current = other.head;
    length = other.length;

    head = new SNode<T>(current->data);
    debug_track_add(head);
    tail    = head;
    current = current->next;

    while (current != nullptr) {
        SNode<T>* newNode = new SNode<T>(current->data);
        debug_track_add(newNode);
        tail->next = newNode;
        tail       = newNode;
        current    = current->next;
    }
}

// front — minimum element (head).  Time: O(1).
template <typename T>
T OrderedLinkedList<T>::front() const {
    assert(head != nullptr);
    return head->data;
}

// back — maximum element (tail).  Time: O(1).
template <typename T>
T OrderedLinkedList<T>::back() const {
    assert(tail != nullptr);
    return tail->data;
}


// ─── Print ────────────────────────────────────────────────────────────────────

// print — forward print (ascending order).  Time: O(n).
template <typename T>
void OrderedLinkedList<T>::print() const {
    SNode<T>* curr = head;
    while (curr != nullptr) {
        cout << curr->data;
        if (curr->next) cout << " ";
        curr = curr->next;
    }
    cout << "\n";
}

// reverse_print — recursive backward print (descending).  Time: O(n).
template <typename T>
void OrderedLinkedList<T>::reverse_print() const {
    reverse_print_helper(head);
    cout << "\n";
}


// ─── Search ───────────────────────────────────────────────────────────────────

// search — sequential search with early exit.
// Book: search() on orderedLinkedList (Ch.5 pp.301-302).
// Time: O(1) best, O(n) worst.
template <typename T>
bool OrderedLinkedList<T>::search(const T& val) const {
    SNode<T>* curr = head;
    while (curr != nullptr && curr->data <= val) {
        if (curr->data == val) return true;
        curr = curr->next;
    }
    return false;
}

// find_by_value — pointer to first matching node, or nullptr.  Early exit.
// Time: O(1) best, O(n) worst.
template <typename T>
SNode<T>* OrderedLinkedList<T>::find_by_value(T val) const {
    SNode<T>* curr = head;
    while (curr != nullptr && curr->data <= val) {
        if (curr->data == val) return curr;
        curr = curr->next;
    }
    return nullptr;
}

// find_index_of — 0-based index of first match, or -1.  Early exit.
// Time: O(1) best, O(n) worst.
template <typename T>
int OrderedLinkedList<T>::find_index_of(T val) const {
    int index = 0;
    SNode<T>* curr = head;
    while (curr != nullptr && curr->data <= val) {
        if (curr->data == val) return index;
        curr = curr->next;
        index++;
    }
    return -1;
}

// binary_search — conceptual only; still O(n) because reaching the midpoint
// requires O(n/2) traversal of a linked list.  Time: O(n).
template <typename T>
bool OrderedLinkedList<T>::binary_search(const T& val) const {
    if (head == nullptr) return false;
    if (val < head->data || val > tail->data) return false;

    int lo = 0, hi = length - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        SNode<T>* curr = head;
        for (int i = 0; i < mid; i++) curr = curr->next;
        if      (curr->data == val) return true;
        else if (val > curr->data ) lo = mid + 1;
        else                        hi = mid - 1;
    }
    return false;
}


// ─── Insertion ────────────────────────────────────────────────────────────────

// insert — insert val in its correct sorted position.
// Book: insert() on orderedLinkedList (Ch.5 pp.303-307).
// Time: O(1) best (val is smallest), O(n) worst (val is largest).
template <typename T>
void OrderedLinkedList<T>::insert(const T& val) {
    SNode<T>* newNode = new SNode<T>(val);
    debug_track_add(newNode);

    if (head == nullptr) { head = tail = newNode; length++; return; }

    // New minimum — insert before head
    if (val <= head->data) { newNode->next = head; head = newNode; length++; return; }

    // New maximum — insert after tail
    if (val >= tail->data) { tail->next = newNode; tail = newNode; length++; return; }

    // Walk to find the first node whose data > val
    SNode<T>* prev = head;
    SNode<T>* curr = head->next;
    while (curr != nullptr && val > curr->data) { prev = curr; curr = curr->next; }

    newNode->next = curr;
    prev->next    = newNode;
    length++;
}


// ─── Deletion ─────────────────────────────────────────────────────────────────

// pop_front — remove minimum (head).  Time: O(1).
template <typename T>
void OrderedLinkedList<T>::pop_front() {
    if (head == nullptr) return;
    SNode<T>* temp = head;
    head = head->next;
    if (head == nullptr) tail = nullptr;
    debug_track_remove(temp);
    delete temp;
    length--;
}

// pop_back — remove maximum (tail).
// Time: O(n) — singly linked; must find predecessor.
template <typename T>
void OrderedLinkedList<T>::pop_back() {
    if (head == nullptr) return;
    if (head == tail)    { pop_front(); return; }

    SNode<T>* prev = head;
    while (prev->next != tail) prev = prev->next;

    debug_track_remove(tail);
    delete tail;
    prev->next = nullptr;
    tail       = prev;
    length--;
}

// remove_at — delete node at 1-based position.
// Time: O(1) best (index == 1), O(n) worst.
template <typename T>
void OrderedLinkedList<T>::remove_at(int index) {
    if (index < 1 || index > length) {
        cout << "remove_at: index " << index << " out of range\n";
        return;
    }
    if (index == 1)      { pop_front(); return; }
    if (index == length) { pop_back();  return; }

    SNode<T>* prev = head;
    for (int i = 1; i < index - 1; i++) prev = prev->next;

    SNode<T>* temp = prev->next;
    prev->next = temp->next;
    if (temp == tail) tail = prev;
    debug_track_remove(temp);
    delete temp;
    length--;
}

// remove_value — remove first node whose data == val.  Early exit.
// Time: O(1) best, O(n) worst.
template <typename T>
void OrderedLinkedList<T>::remove_value(const T& val) {
    if (head == nullptr) return;
    if (head->data == val) { pop_front(); return; }

    SNode<T>* prev = head;
    SNode<T>* curr = head->next;
    while (curr != nullptr && curr->data <= val) {
        if (curr->data == val) {
            prev->next = curr->next;
            if (curr == tail) tail = prev;
            debug_track_remove(curr);
            delete curr;
            length--;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

// remove_all_occurrences — remove every node matching val.
// Duplicates are contiguous in a sorted list.  Early exit.  Time: O(n).
template <typename T>
void OrderedLinkedList<T>::remove_all_occurrences(const T& val) {
    while (head != nullptr && head->data == val) pop_front();
    if (head == nullptr) return;

    SNode<T>* prev = head;
    SNode<T>* curr = head->next;
    while (curr != nullptr && curr->data <= val) {
        if (curr->data == val) {
            prev->next = curr->next;
            if (curr == tail) tail = prev;
            debug_track_remove(curr);
            SNode<T>* temp = curr;
            curr = curr->next;
            delete temp;
            length--;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}


// ─── Merge ────────────────────────────────────────────────────────────────────

// merge — classic O(m+n) sorted merge.  other is consumed (left empty).
// Book: mergeList() (Ch.5 Programming Exercise).
template <typename T>
void OrderedLinkedList<T>::merge(OrderedLinkedList<T>& other) {
    if (other.head == nullptr) return;
    if (head == nullptr) {
        head   = other.head;
        tail   = other.tail;
        length = other.length;
        debug_data.insert(debug_data.end(),
                          other.debug_data.begin(), other.debug_data.end());
        other.head = other.tail = nullptr;
        other.length = 0;
        other.debug_data.clear();
        return;
    }

    SNode<T>* a = head;
    SNode<T>* b = other.head;
    SNode<T>* mergedHead = nullptr;
    SNode<T>* mergedTail = nullptr;

    auto attach = [&](SNode<T>* node) {
        if (mergedHead == nullptr) mergedHead = mergedTail = node;
        else                       { mergedTail->next = node; mergedTail = node; }
        mergedTail->next = nullptr;
    };

    while (a != nullptr && b != nullptr) {
        if (a->data <= b->data) { SNode<T>* next = a->next; attach(a); a = next; }
        else                    { SNode<T>* next = b->next; attach(b); b = next; }
    }
    while (a != nullptr) { SNode<T>* next = a->next; attach(a); a = next; }
    while (b != nullptr) { SNode<T>* next = b->next; attach(b); b = next; }

    head   = mergedHead;
    tail   = mergedTail;
    length = length + other.length;

    debug_data.insert(debug_data.end(),
                      other.debug_data.begin(), other.debug_data.end());
    other.head = other.tail = nullptr;
    other.length = 0;
    other.debug_data.clear();
}


// ─── Access ───────────────────────────────────────────────────────────────────

// get_nth_from_front — pointer to n-th node (1-based).
// Time: O(1) best (n == 1 or n == length), O(n) worst.
template <typename T>
SNode<T>* OrderedLinkedList<T>::get_nth_from_front(int n) const {
    if (n < 1 || n > length) return nullptr;
    if (n == 1)      return head;
    if (n == length) return tail;
    SNode<T>* curr = head;
    for (int i = 1; i < n; i++) curr = curr->next;
    return curr;
}

// get_nth_from_back — pointer to n-th node from the back (1-based).
// Time: O(n).
template <typename T>
SNode<T>* OrderedLinkedList<T>::get_nth_from_back(int n) const {
    if (n < 1 || n > length) return nullptr;
    return get_nth_from_front(length - n + 1);
}

// get_kth_element — data at 1-based position k.  Terminates if OOB.
// Time: O(n).
template <typename T>
T OrderedLinkedList<T>::get_kth_element(int k) const {
    SNode<T>* node = get_nth_from_front(k);
    if (node == nullptr) {
        cerr << "get_kth_element: index " << k << " out of range\n";
        exit(1);
    }
    return node->data;
}

// delete_kth_element — delete node at 1-based position k.  Time: O(n).
template <typename T>
void OrderedLinkedList<T>::delete_kth_element(int k) {
    if (k < 1 || k > length) {
        cout << "delete_kth_element: index " << k << " out of range\n";
        return;
    }
    remove_at(k);
}


// ─── Equality ─────────────────────────────────────────────────────────────────

// equals — element-by-element comparison.  Time: O(n).
template <typename T>
bool OrderedLinkedList<T>::equals(const OrderedLinkedList<T>& other) const {
    if (length != other.length) return false;
    SNode<T>* a = head;
    SNode<T>* b = other.head;
    while (a != nullptr) {
        if (a->data != b->data) return false;
        a = a->next;
        b = b->next;
    }
    return true;
}

template <typename T>
bool OrderedLinkedList<T>::operator==(const OrderedLinkedList<T>& other) const {
    return equals(other);
}


// ─── Utilities ────────────────────────────────────────────────────────────────

// divide_mid — split into two sorted halves (tortoise-and-hare).
// Book: divideMid, Ch.5 Programming Exercise 4 (p.349).  Time: O(n).
template <typename T>
void OrderedLinkedList<T>::divide_mid(OrderedLinkedList<T>& sublist) {
    if (head == nullptr) return;
    SNode<T>* slow = head;
    SNode<T>* fast = head;
    while (fast->next != nullptr && fast->next->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }
    sublist.head   = slow->next;
    sublist.tail   = tail;
    sublist.length = length / 2;

    slow->next = nullptr;
    tail       = slow;
    length     = length - sublist.length;

    sublist.debug_data.clear();
    for (SNode<T>* curr = sublist.head; curr != nullptr; curr = curr->next) {
        sublist.debug_data.push_back(curr);
        auto it = find(debug_data.begin(), debug_data.end(), curr);
        if (it != debug_data.end()) debug_data.erase(it);
    }
}

// count — occurrences of val.  Early exit when data > val.
// Time: O(k) where k is the number of occurrences.
template <typename T>
int OrderedLinkedList<T>::count(const T& val) const {
    int c = 0;
    for (SNode<T>* curr = head; curr != nullptr && curr->data <= val; curr = curr->next)
        if (curr->data == val) c++;
    return c;
}

// is_sorted — verify non-decreasing order.  Time: O(n).
template <typename T>
bool OrderedLinkedList<T>::is_sorted() const {
    for (SNode<T>* curr = head; curr != nullptr && curr->next != nullptr; curr = curr->next)
        if (curr->data > curr->next->data) return false;
    return true;
}

// swap_pairs — swaps data values in adjacent pairs.  Time: O(n).
template <typename T>
void OrderedLinkedList<T>::swap_pairs() {
    if (length <= 1) return;
    for (SNode<T>* curr = head;
         curr != nullptr && curr->next != nullptr;
         curr = curr->next->next)
        swap(curr->data, curr->next->data);
}

// rotate — move head to tail.  Time: O(1).
template <typename T>
void OrderedLinkedList<T>::rotate() {
    if (length <= 1) return;
    SNode<T>* first = head;
    head        = first->next;
    first->next = nullptr;
    tail->next  = first;
    tail        = first;
    debug_verify_integrity();
}

// reverse — in-place three-pointer reversal.
// WARNING: list is in descending order afterwards.
// Time: O(n), Space: O(1).
template <typename T>
void OrderedLinkedList<T>::reverse() {
    if (length <= 1) return;
    tail = head;
    SNode<T>* prev = nullptr;
    SNode<T>* curr = head;
    while (curr != nullptr) {
        SNode<T>* next = curr->next;
        curr->next     = prev;
        prev           = curr;
        curr           = next;
    }
    head = prev;
    debug_verify_integrity();
}


// ─── Debug ────────────────────────────────────────────────────────────────────

template <typename T>
void OrderedLinkedList<T>::debug_print_addresses() const {
    for (SNode<T>* curr = head; curr; curr = curr->next)
        cout << curr << "," << curr->data << "\t";
    cout << "\n";
}

template <typename T>
void OrderedLinkedList<T>::debug_print_node(SNode<T>* node, bool is_separate) const {
    if (is_separate) cout << "Sep: ";
    if (node == nullptr) { cout << "nullptr\n"; return; }
    cout << node->data << " ";
    cout << (node->next ? to_string(node->next->data) : "X") << " ";
    if      (node == head) cout << "head\n";
    else if (node == tail) cout << "tail\n";
    else                   cout << "\n";
}

template <typename T>
void OrderedLinkedList<T>::debug_print_list(string msg) const {
    if (!msg.empty()) cout << msg << "\n";
    for (auto vp : debug_data)
        debug_print_node(static_cast<SNode<T>*>(vp));
    cout << "************\n" << flush;
}

template <typename T>
string OrderedLinkedList<T>::debug_to_string() const {
    if (length == 0) return "";
    ostringstream oss;
    for (SNode<T>* curr = head; curr; curr = curr->next) {
        oss << curr->data;
        if (curr->next) oss << " ";
    }
    return oss.str();
}

template <typename T>
void OrderedLinkedList<T>::debug_verify_integrity() const {
    if (length == 0) { assert(head == nullptr); assert(tail == nullptr); }
    else {
        assert(head != nullptr);
        assert(tail != nullptr);
        if (length == 1) assert(head == tail);
        else             assert(head != tail);
        assert(!tail->next);
    }
    int len = 0;
    for (SNode<T>* curr = head; curr; curr = curr->next, len++) assert(len < 10000);
    assert(length == len);
    assert(length == (int)debug_data.size());

    // Sorted invariant
    for (SNode<T>* curr = head; curr != nullptr && curr->next != nullptr; curr = curr->next)
        assert(curr->data <= curr->next->data);
}
