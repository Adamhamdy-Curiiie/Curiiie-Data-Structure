#include <iostream>
#include <cassert>
#include "DoublyLinkedList.h"
using namespace std;

//  TIME COMPLEXITY — Doubly Linked List
//
//  Operation           Best    Avg     Worst   Doubly linked   Singly linked
// ─────────────────────────────────────────────────────────────────────────────
//  push_front / back   O(1)    O(1)    O(1)    O(1)            O(1)
//  insert_at(i)        O(1)    O(n/2)  O(n)    O(n/2)*         O(n)
//  pop_front           O(1)    O(1)    O(1)    O(1)            O(1)
//  pop_back            O(1)    O(1)    O(1)    O(1) ←BIG WIN   O(n)
//  remove_at(i)        O(1)    O(n/2)  O(n)    O(n/2)*         O(n)
//  remove_min          O(n)    O(n)    O(n)    O(n)            O(n)
//  search / find       O(1)    O(n)    O(n)    same            same
//  get_nth_from_front  O(1)    O(n/2)  O(n)    O(n/2)*         O(n)
//  get_nth_from_back   O(1)    O(n/2)  O(n)    O(n/2)* ←WIN    O(n)
//  rotate_backward     O(1)    O(1)    O(1)    O(1)  ←NEW      N/A
//  print_backward      O(n)    O(n)    O(n)    O(n) no stack   O(n)+stack
//  reverse / swap_pairs O(n)  O(n)    O(n)    O(n)            O(n)
//
//  * walks from whichever end is closer → effectively O(n/4) on average


// ─── Constructors / Destructor ────────────────────────────────────────────────

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList()
    : LinkedListBase<T>(), head(nullptr), tail(nullptr) {}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(T val)
    : LinkedListBase<T>(), head(new DNode<T>(val)), tail(nullptr) {
    tail   = head;
    length = 1;
    debug_track_add(head);
}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList<T>& other)
    : LinkedListBase<T>(), head(nullptr), tail(nullptr) {
    copy_list(other);
}

template <typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(const DoublyLinkedList<T>& other) {
    if (this != &other) { destroy(); copy_list(other); }
    return *this;
}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    DNode<T>* curr = head;
    while (curr != nullptr) {
        DNode<T>* next = curr->next;
        delete curr;
        curr = next;
    }
}


// ─── Core ADT ─────────────────────────────────────────────────────────────────

// destroy — free every node; reset head / tail / length.  Time: O(n).
template <typename T>
void DoublyLinkedList<T>::destroy() {
    DNode<T>* temp;
    while (head != nullptr) {
        temp = head;
        head = head->next;
        debug_track_remove(temp);
        delete temp;
    }
    tail   = nullptr;
    length = 0;
}

// copy_list — node-for-node copy from other.
// Sets both next and prev pointers correctly.  Time: O(n).
template <typename T>
void DoublyLinkedList<T>::copy_list(const DoublyLinkedList<T>& other) {
    if (head != nullptr) destroy();
    if (other.head == nullptr) { head = tail = nullptr; length = 0; return; }

    DNode<T>* current = other.head;
    length = other.length;

    head = new DNode<T>(current->data);
    debug_track_add(head);
    tail    = head;
    current = current->next;

    while (current != nullptr) {
        DNode<T>* newNode = new DNode<T>(current->data);
        debug_track_add(newNode);
        newNode->prev = tail;
        tail->next    = newNode;
        tail          = newNode;
        current       = current->next;
    }
}

// front — data of the first node.  Time: O(1).
template <typename T>
T DoublyLinkedList<T>::front() const {
    assert(head != nullptr);
    return head->data;
}

// back — data of the last node.  Time: O(1).
template <typename T>
T DoublyLinkedList<T>::back() const {
    assert(tail != nullptr);
    return tail->data;
}


// ─── Print ────────────────────────────────────────────────────────────────────

// print — forward print.  Time: O(n).
template <typename T>
void DoublyLinkedList<T>::print() const {
    DNode<T>* curr = head;
    while (curr != nullptr) {
        cout << curr->data;
        if (curr->next) cout << " ";
        curr = curr->next;
    }
    cout << "\n";
}

// print_backward — iterative backward print via prev.  No call stack needed.
// Time: O(n).  Space: O(1).
template <typename T>
void DoublyLinkedList<T>::print_backward() const {
    DNode<T>* curr = tail;
    while (curr != nullptr) {
        cout << curr->data;
        if (curr->prev) cout << " ";
        curr = curr->prev;
    }
    cout << "\n";
}

// reverse_print — recursive backward print (mirrors singly linked API).
// Time: O(n).  Space: O(n).
template <typename T>
void DoublyLinkedList<T>::reverse_print() const {
    reverse_print_helper(head);
    cout << "\n";
}


// ─── Search ───────────────────────────────────────────────────────────────────

// search — sequential search.  Time: O(1) best, O(n) worst.
template <typename T>
bool DoublyLinkedList<T>::search(const T& val) const {
    return find_by_value(val) != nullptr;
}

// find_by_value — pointer to first matching node, or nullptr.
// Time: O(1) best, O(n) worst.
template <typename T>
DNode<T>* DoublyLinkedList<T>::find_by_value(T val) const {
    for (DNode<T>* curr = head; curr != nullptr; curr = curr->next)
        if (curr->data == val) return curr;
    return nullptr;
}

// find_index_of — 0-based index of first match, or -1.
// Time: O(1) best, O(n) worst.
template <typename T>
int DoublyLinkedList<T>::find_index_of(T val) const {
    int index = 0;
    for (DNode<T>* curr = head; curr != nullptr; curr = curr->next, index++)
        if (curr->data == val) return index;
    return -1;
}

// transposition_search — find val, swap its data with its predecessor's.
// In a doubly linked list the swap must update four pointer neighbours,
// but swapping only the data values (not the nodes themselves) keeps it simple.
// Book: transposition heuristic (Ch.9).
// Time: O(1) best, O(n) worst.
template <typename T>
int DoublyLinkedList<T>::transposition_search(T val) {
    DNode<T>* curr  = head;
    int       index = 0;
    while (curr != nullptr) {
        if (curr->data == val) {
            if (curr->prev != nullptr) swap(curr->data, curr->prev->data);
            return (index > 0) ? index - 1 : 0;
        }
        curr = curr->next;
        index++;
    }
    return -1;
}


// ─── Insertion ────────────────────────────────────────────────────────────────

// push_back — append at tail.  Time: O(1).
template <typename T>
void DoublyLinkedList<T>::push_back(const T& val) {
    DNode<T>* newNode = new DNode<T>(val);
    debug_track_add(newNode);
    if (head == nullptr) { head = tail = newNode; }
    else { newNode->prev = tail; tail->next = newNode; tail = newNode; }
    length++;
}

// push_front — prepend at head.  Time: O(1).
template <typename T>
void DoublyLinkedList<T>::push_front(const T& val) {
    DNode<T>* newNode = new DNode<T>(val);
    debug_track_add(newNode);
    if (head == nullptr) { head = tail = newNode; }
    else { newNode->next = head; head->prev = newNode; head = newNode; }
    length++;
}

// insert_at — insert at 1-based position, walking from the closer end.
// Time: O(1) best (index == 1 or index == length+1), O(n/2) avg.
template <typename T>
void DoublyLinkedList<T>::insert_at(int index, const T& val) {
    if (index <= 1)     { push_front(val); return; }
    if (index > length) { push_back(val);  return; }

    DNode<T>* newNode = new DNode<T>(val);
    debug_track_add(newNode);

    if (index <= length / 2) {
        // walk forward from head
        DNode<T>* curr = head;
        for (int i = 1; i < index - 1; i++) curr = curr->next;
        newNode->next    = curr->next;
        newNode->prev    = curr;
        curr->next->prev = newNode;
        curr->next       = newNode;
    } else {
        // walk backward from tail
        DNode<T>* curr = tail;
        for (int i = length; i > index; i--) curr = curr->prev;
        newNode->prev    = curr->prev;
        newNode->next    = curr;
        curr->prev->next = newNode;
        curr->prev       = newNode;
    }
    length++;
}


// ─── Deletion ─────────────────────────────────────────────────────────────────

// pop_front — remove head.  Time: O(1).
template <typename T>
void DoublyLinkedList<T>::pop_front() {
    if (head == nullptr) return;
    DNode<T>* temp = head;
    head = head->next;
    if (head) head->prev = nullptr;
    else      tail       = nullptr;
    debug_track_remove(temp);
    delete temp;
    length--;
}

// pop_back — remove tail.  Time: O(1) — key advantage of doubly linked.
template <typename T>
void DoublyLinkedList<T>::pop_back() {
    if (tail == nullptr) return;
    DNode<T>* temp = tail;
    tail = tail->prev;
    if (tail) tail->next = nullptr;
    else      head       = nullptr;
    debug_track_remove(temp);
    delete temp;
    length--;
}

// remove_at — delete node at 1-based position, walking from the closer end.
// Time: O(1) best (index == 1 or length), O(n/2) avg.
template <typename T>
void DoublyLinkedList<T>::remove_at(int index) {
    if (index < 1 || index > length) {
        cout << "remove_at: index " << index << " out of range\n";
        return;
    }
    if (index == 1)      { pop_front(); return; }
    if (index == length) { pop_back();  return; }

    DNode<T>* curr;
    if (index <= length / 2) {
        curr = head;
        for (int i = 1; i < index; i++) curr = curr->next;
    } else {
        curr = tail;
        for (int i = length; i > index; i--) curr = curr->prev;
    }
    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    debug_track_remove(curr);
    delete curr;
    length--;
}

// remove_value — remove first node whose data == val.
// Time: O(1) best, O(n) worst.
template <typename T>
void DoublyLinkedList<T>::remove_value(const T& val) {
    DNode<T>* curr = head;
    while (curr != nullptr) {
        if (curr->data == val) {
            if (curr == head) { pop_front(); return; }
            if (curr == tail) { pop_back();  return; }
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
            debug_track_remove(curr);
            delete curr;
            length--;
            return;
        }
        curr = curr->next;
    }
}

// remove_min — scan once to find minimum, then delete directly.  Time: O(n).
template <typename T>
void DoublyLinkedList<T>::remove_min() {
    if (head == nullptr) return;
    DNode<T>* minNode = head;
    for (DNode<T>* curr = head->next; curr != nullptr; curr = curr->next)
        if (minNode->data > curr->data) minNode = curr;

    if (minNode == head) { pop_front(); return; }
    if (minNode == tail) { pop_back();  return; }
    minNode->prev->next = minNode->next;
    minNode->next->prev = minNode->prev;
    debug_track_remove(minNode);
    delete minNode;
    length--;
}

// remove_all_occurrences — single traversal via curr->next before deletion.
// Time: O(n).
template <typename T>
void DoublyLinkedList<T>::remove_all_occurrences(const T& val) {
    DNode<T>* curr = head;
    while (curr != nullptr) {
        DNode<T>* next = curr->next;
        if (curr->data == val) {
            if (curr == head)       pop_front();
            else if (curr == tail)  pop_back();
            else {
                curr->prev->next = curr->next;
                curr->next->prev = curr->prev;
                debug_track_remove(curr);
                delete curr;
                length--;
            }
        }
        curr = next;
    }
}


// ─── Access ───────────────────────────────────────────────────────────────────

// get_nth_from_front — pointer to n-th node (1-based), walking from closer end.
// Time: O(1) best (n == 1 or n == length), O(n/2) avg.
template <typename T>
DNode<T>* DoublyLinkedList<T>::get_nth_from_front(int n) const {
    if (n < 1 || n > length) return nullptr;
    if (n == 1)      return head;
    if (n == length) return tail;

    DNode<T>* curr;
    if (n <= length / 2) {
        curr = head;
        for (int i = 1; i < n; i++) curr = curr->next;
    } else {
        curr = tail;
        for (int i = length; i > n; i--) curr = curr->prev;
    }
    return curr;
}

// get_nth_from_back — pointer to n-th node from the back (1-based).
// Walks backward via prev.  Time: O(1) best (n == 1), O(n) worst.
template <typename T>
DNode<T>* DoublyLinkedList<T>::get_nth_from_back(int n) const {
    if (n < 1 || n > length) return nullptr;
    DNode<T>* curr = tail;
    for (int i = 1; i < n; i++) curr = curr->prev;
    return curr;
}

// get_kth_element — data at 1-based position k.  Terminates if OOB.
// Time: O(n).
template <typename T>
T DoublyLinkedList<T>::get_kth_element(int k) const {
    DNode<T>* node = get_nth_from_front(k);
    if (node == nullptr) { cerr << "get_kth_element: index " << k << " out of range\n"; exit(1); }
    return node->data;
}

// delete_kth_element — delete node at 1-based position k.  Time: O(n).
template <typename T>
void DoublyLinkedList<T>::delete_kth_element(int k) {
    if (k < 1 || k > length) {
        cout << "delete_kth_element: index " << k << " out of range\n";
        return;
    }
    remove_at(k);
}


// ─── Equality ─────────────────────────────────────────────────────────────────

// equals — element-by-element comparison.  Time: O(n).
template <typename T>
bool DoublyLinkedList<T>::equals(const DoublyLinkedList<T>& other) const {
    if (length != other.length) return false;
    DNode<T>* a = head;
    DNode<T>* b = other.head;
    while (a != nullptr) {
        if (a->data != b->data) return false;
        a = a->next;
        b = b->next;
    }
    return true;
}

template <typename T>
bool DoublyLinkedList<T>::operator==(const DoublyLinkedList<T>& other) const {
    return equals(other);
}


// ─── Utilities ────────────────────────────────────────────────────────────────

// swap_pairs — swap data values in consecutive pairs.  Time: O(n).
template <typename T>
void DoublyLinkedList<T>::swap_pairs() {
    if (length <= 1) return;
    for (DNode<T>* curr = head;
         curr != nullptr && curr->next != nullptr;
         curr = curr->next->next)
        swap(curr->data, curr->next->data);
}

// divide_mid — split into two halves (tortoise-and-hare).
// Correctly zeros prev pointer of the new sublist head.  Time: O(n).
template <typename T>
void DoublyLinkedList<T>::divide_mid(DoublyLinkedList<T>& sublist) {
    if (head == nullptr) return;
    DNode<T>* slow = head;
    DNode<T>* fast = head;
    while (fast->next != nullptr && fast->next->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }
    sublist.head   = slow->next;
    sublist.tail   = tail;
    sublist.length = length / 2;

    if (sublist.head) sublist.head->prev = nullptr;
    slow->next = nullptr;
    tail       = slow;
    length     = length - sublist.length;

    sublist.debug_data.clear();
    for (DNode<T>* curr = sublist.head; curr != nullptr; curr = curr->next) {
        sublist.debug_data.push_back(curr);
        auto it = find(debug_data.begin(), debug_data.end(), curr);
        if (it != debug_data.end()) debug_data.erase(it);
    }
}

// divide_at — split at the first occurrence of item.
// Uses prev to sever without a predecessor walk.  Time: O(n).
template <typename T>
void DoublyLinkedList<T>::divide_at(DoublyLinkedList<T>& sublist, const T& item) {
    if (head == nullptr) return;

    if (head->data == item) {
        sublist.head   = head;
        sublist.tail   = tail;
        sublist.length = length;
        head = tail = nullptr;
        length = 0;
        sublist.debug_data = debug_data;
        debug_data.clear();
        return;
    }

    DNode<T>* curr = head;
    int pos = 0;
    while (curr != nullptr && curr->data != item) { curr = curr->next; pos++; }

    if (curr == nullptr) { cout << "divide_at: item not found\n"; return; }

    curr->prev->next = nullptr;
    tail             = curr->prev;
    curr->prev       = nullptr;
    sublist.head     = curr;
    sublist.length   = length - pos;
    length           = pos;

    // correct sublist tail
    DNode<T>* tmp = sublist.head;
    while (tmp->next != nullptr) tmp = tmp->next;
    sublist.tail = tmp;

    sublist.debug_data.clear();
    for (DNode<T>* c = sublist.head; c != nullptr; c = c->next) {
        sublist.debug_data.push_back(c);
        auto it = find(debug_data.begin(), debug_data.end(), c);
        if (it != debug_data.end()) debug_data.erase(it);
    }
}

// rotate — move head to tail.  Time: O(1).
template <typename T>
void DoublyLinkedList<T>::rotate() {
    if (length <= 1) return;
    DNode<T>* first = head;
    head       = first->next;
    head->prev = nullptr;
    first->prev = tail;
    first->next = nullptr;
    tail->next  = first;
    tail        = first;
    debug_verify_integrity();
}

// rotate_backward — move tail to head.  Time: O(1).
// Only possible in O(1) because of the prev pointer.
template <typename T>
void DoublyLinkedList<T>::rotate_backward() {
    if (length <= 1) return;
    DNode<T>* last = tail;
    tail       = last->prev;
    tail->next = nullptr;
    last->next = head;
    last->prev = nullptr;
    head->prev = last;
    head       = last;
    debug_verify_integrity();
}

// reverse — in-place reversal by swapping next/prev on every node.
// Time: O(n), Space: O(1).
template <typename T>
void DoublyLinkedList<T>::reverse() {
    if (length <= 1) return;
    DNode<T>* curr = head;
    tail = head;
    while (curr != nullptr) {
        DNode<T>* next = curr->next;
        curr->next = curr->prev;
        curr->prev = next;
        if (next == nullptr) head = curr;
        curr = next;
    }
    debug_verify_integrity();
}


// ─── Debug ────────────────────────────────────────────────────────────────────

template <typename T>
void DoublyLinkedList<T>::debug_print_addresses() const {
    for (DNode<T>* curr = head; curr; curr = curr->next)
        cout << curr << "," << curr->data << "\t";
    cout << "\n";
}

template <typename T>
void DoublyLinkedList<T>::debug_print_node(DNode<T>* node, bool is_separate) const {
    if (is_separate) cout << "Sep: ";
    if (node == nullptr) { cout << "nullptr\n"; return; }
    string prev_str = node->prev ? to_string(node->prev->data) : "X";
    string next_str = node->next ? to_string(node->next->data) : "X";
    cout << prev_str << " <- " << node->data << " -> " << next_str << " ";
    if      (node == head) cout << "head\n";
    else if (node == tail) cout << "tail\n";
    else                   cout << "\n";
}

template <typename T>
void DoublyLinkedList<T>::debug_print_list(string msg) const {
    if (!msg.empty()) cout << msg << "\n";
    for (auto vp : debug_data)
        debug_print_node(static_cast<DNode<T>*>(vp));
    cout << "************\n" << flush;
}

template <typename T>
string DoublyLinkedList<T>::debug_to_string() const {
    if (length == 0) return "";
    ostringstream oss;
    for (DNode<T>* curr = head; curr; curr = curr->next) {
        oss << curr->data;
        if (curr->next) oss << " ";
    }
    return oss.str();
}

template <typename T>
void DoublyLinkedList<T>::debug_verify_integrity() const {
    if (length == 0) { assert(head == nullptr); assert(tail == nullptr); }
    else {
        assert(head != nullptr);
        assert(tail != nullptr);
        assert(head->prev == nullptr);
        assert(tail->next == nullptr);
        if (length == 1) assert(head == tail);
        else             assert(head != tail);
    }
    int len = 0;
    for (DNode<T>* curr = head; curr; curr = curr->next, len++) {
        assert(len < 10000);
        if (curr->prev) assert(curr->prev->next == curr);
        if (curr->next) assert(curr->next->prev == curr);
    }
    assert(length == len);
    assert(length == (int)debug_data.size());
}
