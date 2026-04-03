#include <iostream>
#include <cassert>
#include "LinkedList.h"
using namespace std;

//  TIME COMPLEXITY — Singly Linked List (unordered, with head + tail)
//  Operation           Best    Avg     Worst   Notes
// ─────────────────────────────────────────────────────────────────────────────
//  push_front          O(1)    O(1)    O(1)
//  push_back           O(1)    O(1)    O(1)    tail pointer gives direct access
//  insert_at(i)        O(1)    O(n)    O(n)    O(1) only when i == 1
//  pop_front           O(1)    O(1)    O(1)
//  pop_back            O(n)    O(n)    O(n)    singly linked — need predecessor
//  remove_at(i)        O(1)    O(n)    O(n)    O(1) only when i == 1
//  remove_value(v)     O(1)    O(n)    O(n)
//  remove_min          O(n)    O(n)    O(n)
//  search / find       O(1)    O(n)    O(n)
//  front / back        O(1)    O(1)    O(1)    tail makes back() O(1)
//  rotate()            O(1)    O(1)    O(1)    tail makes this O(1)
//  reverse / swap_pairs O(n)  O(n)    O(n)
//  copy / destroy      O(n)    O(n)    O(n)


// ─── Constructors / Destructor ────────────────────────────────────────────────

template <typename T>
LinkedList<T>::LinkedList() : LinkedListBase<T>(), head(nullptr), tail(nullptr) {}

template <typename T>
LinkedList<T>::LinkedList(T val)
    : LinkedListBase<T>(), head(new SNode<T>(val)), tail(nullptr) {
    tail = head;
    length = 1;
    debug_track_add(head);
}

// Time: O(n).
template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& other)
    : LinkedListBase<T>(), head(nullptr), tail(nullptr) {
    copy_list(other);
}

// Time: O(n).
template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other) {
    if (this != &other) {
        destroy();
        copy_list(other);
    }
    return *this;
}

template <typename T>
LinkedList<T>::~LinkedList() {
    SNode<T>* curr = head;
    while (curr != nullptr) {
        SNode<T>* next = curr->next;
        delete curr;
        curr = next;
    }
}



// destroy — free every node; reset head / tail / length.
template <typename T>
void LinkedList<T>::destroy() {
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
template <typename T>
void LinkedList<T>::copy_list(const LinkedList<T>& other) {
    if (head != nullptr)
        destroy();

    if (other.head == nullptr) {
        head = tail = nullptr;
        length = 0;
        return;
    }

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
        tail = newNode;
        current = current->next;
    }
}

template <typename T>
T LinkedList<T>::front() const {
    assert(head != nullptr);
    return head->data;
}

// back — data of the last node.
template <typename T>
T LinkedList<T>::back() const {
    assert(tail != nullptr);
    return tail->data;
}


// ─── Print ────────────────────────────────────────────────────────────────────

// print — forward print.  Time: O(n).
template <typename T>
void LinkedList<T>::print() const {
    for (SNode<T>* curr = head; curr != nullptr; curr = curr->next)
        cout << curr->data << " ";
    cout << "\n";
}

// reverse_print — recursive backward print.
template <typename T>
void LinkedList<T>::reverse_print() const {
    reverse_print_helper(head);
    cout << "\n";
}


// ─── Search ───────────────────────────────────────────────────────────────────

// search — sequential search.
// Book: search() on unorderedLinkedList (p.293-294).
// Time: O(1) best, O(n) worst.
template <typename T>
bool LinkedList<T>::search(const T& val) const {
    for (SNode<T>* curr = head; curr != nullptr; curr = curr->next)
        if (curr->data == val) return true;
    return false;
}

// find_by_value — pointer to first matching node, or nullptr.
// Time: O(1) best, O(n) worst.
template <typename T>
SNode<T>* LinkedList<T>::find_by_value(T val) const {
    for (SNode<T>* curr = head; curr != nullptr; curr = curr->next)
        if (curr->data == val) return curr;
    return nullptr;
}

// find_index_of — 0-based index of first match, or -1.
// Time: O(1) best, O(n) worst.
template <typename T>
int LinkedList<T>::find_index_of(T val) const {
    int index = 0;
    for (SNode<T>* curr = head; curr != nullptr; curr = curr->next, index++)
        if (curr->data == val) return index;
    return -1;
}

// transposition_search — find val, then swap its data with its predecessor's.
// Repeated searches move hot items toward the head.
// Book: transposition heuristic (Ch.9).
// Time: O(1) best, O(n) worst.  Side-effect: reorders data values.
template <typename T>
int LinkedList<T>::transposition_search(T val) {
    int index = 0;
    SNode<T>* prev = nullptr;
    for (SNode<T>* curr = head; curr != nullptr; curr = curr->next, index++) {
        if (curr->data == val) {
            if (prev == nullptr) return index;   // already at the front
            swap(prev->data, curr->data);
            return index - 1;
        }
        prev = curr;
    }
    return -1;
}


// ─── Insertion ────────────────────────────────────────────────────────────────

// push_back — append at tail.
// Book: insertLast (p.294-295).  Time: O(1).
template <typename T>
void LinkedList<T>::push_back(const T& val) {
    SNode<T>* node = new SNode<T>(val);
    debug_track_add(node);
    if (head == nullptr) { head = tail = node; }
    else                 { tail->next = node; tail = node; }
    length++;
    debug_verify_integrity();
}

// push_front — prepend at head.
// Book: insertFirst (p.294).  Time: O(1).
template <typename T>
void LinkedList<T>::push_front(const T& val) {
    SNode<T>* node = new SNode<T>(val);
    debug_track_add(node);
    if (head == nullptr) { head = tail = node; }
    else                 { node->next = head; head = node; }
    length++;
    debug_verify_integrity();
}

// insert_at — insert at 1-based position.
// Delegates to push_front / push_back at the edges.
// Time: O(1) best (index == 1), O(n) worst.
template <typename T>
void LinkedList<T>::insert_at(int index, const T& val) {
    if (index < 1 || index > length + 1) {
        cout << "insert_at: invalid index " << index << "\n";
        return;
    }
    if (index == 1)          { push_front(val); return; }
    if (index == length + 1) { push_back(val);  return; }

    SNode<T>* prev = get_nth_from_front(index - 1);
    SNode<T>* node = new SNode<T>(val);
    debug_track_add(node);
    node->next = prev->next;
    prev->next = node;
    length++;
    debug_verify_integrity();
}


// ─── Deletion ─────────────────────────────────────────────────────────────────

// pop_front — remove head.  Time: O(1).
template <typename T>
void LinkedList<T>::pop_front() {
    if (head == nullptr) return;
    SNode<T>* temp = head;
    debug_track_remove(temp);
    if (head == tail) head = tail = nullptr;
    else              head = head->next;
    delete temp;
    length--;
    debug_verify_integrity();
}

// pop_back — remove tail.  Time: O(n) — singly linked, need predecessor.
template <typename T>
void LinkedList<T>::pop_back() {
    if (length <= 1) { pop_front(); return; }
    debug_track_remove(tail);
    SNode<T>* prev = get_nth_from_front(length - 1);
    delete tail;
    tail       = prev;
    tail->next = nullptr;
    length--;
    debug_verify_integrity();
}

// remove_at — delete node at 1-based position.
// Book: deleteNode pattern (p.296-298).
// Time: O(1) at head, O(n) general.
template <typename T>
void LinkedList<T>::remove_at(int index) {
    if (index < 1 || index > length) return;
    if (index == 1)      { pop_front(); return; }
    if (index == length) { pop_back();  return; }
    SNode<T>* prev = get_nth_from_front(index - 1);
    SNode<T>* curr = prev->next;
    debug_track_remove(curr);
    prev->next = curr->next;
    delete curr;
    length--;
    debug_verify_integrity();
}

// remove_value — delete first node whose data == val.
// Book: deleteNode on unorderedLinkedList (p.296-298).
// Time: O(1) best, O(n) worst.
template <typename T>
void LinkedList<T>::remove_value(const T& val) {
    int pos = find_index_of(val) + 1;   // convert 0-based to 1-based
    if (pos == 0) return;
    remove_at(pos);
    debug_verify_integrity();
}

// remove_min — find and remove the smallest element.
// Book: Programming Exercise 2a, Ch.5 (p.349).  Time: O(n).
template <typename T>
void LinkedList<T>::remove_min() {
    if (head == nullptr) return;
    SNode<T>* minPrev = nullptr;
    SNode<T>* minNode = head;
    SNode<T>* prev    = nullptr;
    for (SNode<T>* curr = head; curr != nullptr; curr = curr->next) {
        if (minNode->data > curr->data) { minNode = curr; minPrev = prev; }
        prev = curr;
    }
    if      (minNode == head) pop_front();
    else if (minNode == tail) pop_back();
    else {
        debug_track_remove(minNode);
        minPrev->next = minNode->next;
        delete minNode;
        length--;
        debug_verify_integrity();
    }
}

// remove_all_occurrences — delete every node whose data == val.
// Book: Programming Exercise 2b, Ch.5 (p.349).  Time: O(n).
template <typename T>
void LinkedList<T>::remove_all_occurrences(const T& val) {
    while (head != nullptr && head->data == val) pop_front();
    if (head == nullptr) return;
    SNode<T>* prev = head;
    SNode<T>* curr = head->next;
    while (curr != nullptr) {
        if (curr->data == val) {
            prev->next = curr->next;
            if (curr == tail) tail = prev;
            debug_track_remove(curr);
            delete curr;
            curr = prev->next;
            length--;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
    debug_verify_integrity();
}


// ─── Access ───────────────────────────────────────────────────────────────────

// get_nth_from_front — pointer to the n-th node (1-based).
// Time: O(1) best (n == 1), O(n) worst.
template <typename T>
SNode<T>* LinkedList<T>::get_nth_from_front(int n) const {
    if (n < 1 || n > length) {
        cout << "get_nth_from_front: invalid position " << n << "\n";
        return nullptr;
    }
    SNode<T>* curr = head;
    for (int i = 1; i < n; i++) curr = curr->next;
    return curr;
}

// get_nth_from_back — pointer to n-th node from the end (1-based).
// Time: O(n).
template <typename T>
SNode<T>* LinkedList<T>::get_nth_from_back(int n) const {
    return get_nth_from_front(length - n + 1);
}

// get_kth_element — data at 1-based position k.
// Book: Programming Exercise 3a, Ch.5 (p.349).  Time: O(n).
template <typename T>
T LinkedList<T>::get_kth_element(int k) const {
    SNode<T>* node = get_nth_from_front(k);
    assert(node != nullptr);
    return node->data;
}

// delete_kth_element — delete node at 1-based position k.
// Book: Programming Exercise 3b, Ch.5 (p.349).  Time: O(n).
template <typename T>
void LinkedList<T>::delete_kth_element(int k) {
    if (k < 1 || k > length) {
        cout << "delete_kth_element: no element at position " << k << "\n";
        return;
    }
    remove_at(k);
}


// ─── Equality ─────────────────────────────────────────────────────────────────

// operator== — element-by-element comparison.  Time: O(n).
template <typename T>
bool LinkedList<T>::operator==(const LinkedList<T>& other) const {
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


// ─── Utilities ────────────────────────────────────────────────────────────────

// swap_pairs — swap data values in consecutive pairs in-place.  Time: O(n).
template <typename T>
void LinkedList<T>::swap_pairs() {
    if (length <= 1) return;
    for (SNode<T>* curr = head;
         curr != nullptr && curr->next != nullptr;
         curr = curr->next->next)
        swap(curr->data, curr->next->data);
}

// divide_mid — split into two nearly-equal halves (tortoise-and-hare).
// "this" keeps the first half; sublist receives the second.
// Book: divideMid, Programming Exercise 4 (p.349).  Time: O(n).
template <typename T>
void LinkedList<T>::divide_mid(LinkedList<T>& sublist) {
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

    // sync debug trackers
    sublist.debug_data.clear();
    for (SNode<T>* curr = sublist.head; curr != nullptr; curr = curr->next) {
        sublist.debug_data.push_back(curr);
        auto it = find(debug_data.begin(), debug_data.end(), curr);
        if (it != debug_data.end()) debug_data.erase(it);
    }
}

// divide_at — split at the first occurrence of item.
// "this" keeps nodes before item; sublist starts at item.
template <typename T>
void LinkedList<T>::divide_at(LinkedList<T>& sublist, const T& item) {
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

    SNode<T>* prev = nullptr;
    SNode<T>* curr = head;
    int pos = 0;
    while (curr != nullptr && curr->data != item) { prev = curr; curr = curr->next; pos++; }

    if (curr == nullptr) { cout << "divide_at: item not found\n"; return; }

    prev->next     = nullptr;
    sublist.head   = curr;
    sublist.length = length - pos;
    length         = pos;

    // correct both tails with a single walk each
    SNode<T>* tmp = head;
    while (tmp->next != nullptr) tmp = tmp->next;
    tail = tmp;

    SNode<T>* tmp2 = sublist.head;
    while (tmp2->next != nullptr) tmp2 = tmp2->next;
    sublist.tail = tmp2;

    sublist.debug_data.clear();
    for (SNode<T>* c = sublist.head; c != nullptr; c = c->next) {
        sublist.debug_data.push_back(c);
        auto it = find(debug_data.begin(), debug_data.end(), c);
        if (it != debug_data.end()) debug_data.erase(it);
    }
}

// rotate — move head to tail.
// Book: rotate()
template <typename T>
void LinkedList<T>::rotate() {
    if (length <= 1) return;
    SNode<T>* first = head;
    head = first->next;
    first->next = nullptr;
    tail->next  = first;
    tail = first;
    debug_verify_integrity();
}

// reverse — in-place three-pointer reversal.
// Time: O(n), Space: O(1).
template <typename T>
void LinkedList<T>::reverse() {
    if (length <= 1) return;
    tail = head;
    SNode<T>* prev = nullptr;
    SNode<T>* curr = head;
    while (curr != nullptr) {
        SNode<T>* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    head = prev;
    debug_verify_integrity();
}


// ─── Debug ────────────────────────────────────────────────────────────────────

template <typename T>
void LinkedList<T>::debug_print_addresses() const {
    for (SNode<T>* curr = head; curr; curr = curr->next)
        cout << curr << "," << curr->data << "\t";
    cout << "\n";
}

template <typename T>
void LinkedList<T>::debug_print_node(SNode<T>* node, bool is_separate) const {
    if (is_separate) cout << "Sep: ";
    if (node == nullptr) { cout << "nullptr\n"; return; }
    cout << node->data << " ";
    cout << (node->next ? to_string(node->next->data) : "X") << " ";
    if      (node == head) cout << "head\n";
    else if (node == tail) cout << "tail\n";
    else                   cout << "\n";
}

template <typename T>
void LinkedList<T>::debug_print_list(string msg) const {
    if (!msg.empty()) cout << msg << "\n";
    for (auto vp : debug_data)
        debug_print_node(static_cast<SNode<T>*>(vp));
    cout << "************\n" << flush;
}

template <typename T>
string LinkedList<T>::debug_to_string() const {
    if (length == 0) return "";
    ostringstream oss;
    for (SNode<T>* curr = head; curr; curr = curr->next) {
        oss << curr->data;
        if (curr->next) oss << " ";
    }
    return oss.str();
}

template <typename T>
void LinkedList<T>::debug_verify_integrity() const {
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
}
