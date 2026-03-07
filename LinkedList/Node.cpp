#include <iostream>
#include <cassert>
#include "Node.h"
using namespace std;

// ─── Constructors / Destructor ────────────────────────────────────────────────

template <typename T>
LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), length(0) {}

template <typename T>
LinkedList<T>::LinkedList(T val) : head(new Node<T>(val)), length(1) {
    tail = head;
    debug_track_add(head);
}

template <typename T>
LinkedList<T>::~LinkedList() {
    Node<T>* curr = head;
    while (curr != nullptr) {
        Node<T>* next = curr->next;
        delete curr;
        curr = next;
    }
}

// ─── Print ────────────────────────────────────────────────────────────────────

template <typename T>
void LinkedList<T>::print() const {
    for (Node<T>* curr = head; curr != nullptr; curr = curr->next)
        cout << curr->data << " ";
    cout << "\n";
}

// ─── Search ───────────────────────────────────────────────────────────────────

template <typename T>
Node<T>* LinkedList<T>::find_by_value(T val) const {
    for (Node<T>* curr = head; curr != nullptr; curr = curr->next)
        if (curr->data == val)
            return curr;
    return nullptr;
}

template <typename T>
T LinkedList<T>::find_index_of(int val) const {
    int index = 0;
    for (Node<T>* curr = head; curr != nullptr; curr = curr->next, index++)
        if (curr->data == val)
            return index;
    return -1;
}

template <typename T>
int LinkedList<T>::transposition_search(T val) {
    int index = 0;
    Node<T>* prev = nullptr;

    for (Node<T>* curr = head; curr != nullptr; curr = curr->next, index++) {
        if (curr->data == val) {
            if (prev == nullptr)
                return index;
            swap(prev->data, curr->data);
            return index - 1;
        }
        prev = curr;
    }
    return -1;
}

// ─── Insertion ────────────────────────────────────────────────────────────────

template <typename T>
void LinkedList<T>::push_back(const T& val) {
    Node<T>* node = new Node<T>(val);
    debug_track_add(node);

    if (head == nullptr) {
        head = tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
    length++;
    debug_verify_integrity();
}

template <typename T>
void LinkedList<T>::push_front(const T& val) {
    Node<T>* node = new Node<T>(val);
    debug_track_add(node);

    if (head == nullptr) {
        head = tail = node;
    } else {
        node->next = head;
        head = node;
    }
    length++;
    debug_verify_integrity();
}

// ─── Deletion ─────────────────────────────────────────────────────────────────

template <typename T>
void LinkedList<T>::pop_front() {
    if (head == nullptr)
        return;

    Node<T>* temp = head;
    debug_track_remove(temp);

    if (head == tail) {
        head = tail = nullptr;
    } else {
        head = head->next;
    }

    delete temp;
    length--;
    debug_verify_integrity();
}

template <typename T>
void LinkedList<T>::pop_back() {
    if (length <= 1) {
        pop_front();
        return;
    }

    debug_track_remove(tail);
    Node<T>* prev = get_nth_from_front(length - 1);
    delete tail;
    tail = prev;
    tail->next = nullptr;
    length--;
    debug_verify_integrity();
}

template <typename T>
void LinkedList<T>::remove_at(int index) {
    if (index < 1 || index > length)
        return;

    if (index == 1) {
        pop_front();
        return;
    }
    if (index == length) {
        pop_back();
        return;
    }

    Node<T>* prev = get_nth_from_front(index - 1);
    Node<T>* curr = prev->next;
    debug_track_remove(curr);
    prev->next = curr->next;
    delete curr;
    length--;
    debug_verify_integrity();
}

template <typename T>
void LinkedList<T>::remove_value(int val) {
    int pos = find_index_of(val) + 1;
    if (pos == 0)
        return;
    remove_at(pos);
    debug_verify_integrity();
}

// ─── Access ───────────────────────────────────────────────────────────────────

template <typename T>
Node<T>* LinkedList<T>::get_nth_from_front(T n) const {
    if (n < 1 || n > length) {
        cout << "Invalid position: " << n << "\n";
        return nullptr;
    }

    Node<T>* curr = head;
    for (int i = 1; i < n; i++)
        curr = curr->next;

    return curr;
}

template <typename T>
Node<T>* LinkedList<T>::get_nth_from_back(T n) const {
    return get_nth_from_front(length - n + 1);
}

// ─── Equality ─────────────────────────────────────────────────────────────────

template <typename T>
bool LinkedList<T>::equals(LinkedList<T>& other, int other_length) const {
    if (this->length != other_length)
        return false;

    Node<T>* a = head;
    Node<T>* b = other.head;
    while (a != nullptr && b != nullptr) {
        if (a->data != b->data)
            return false;
        a = a->next;
        b = b->next;
    }
    return true;
}

template <typename T>
bool LinkedList<T>::equals(LinkedList<T>& other) const {
    Node<T>* a = head;
    Node<T>* b = other.head;
    while (a != nullptr && b != nullptr) {
        if (a->data != b->data)
            return false;
        a = a->next;
        b = b->next;
    }
    return a == nullptr && b == nullptr;
}

// ─── Utilities ────────────────────────────────────────────────────────────────

template <typename T>
void LinkedList<T>::swap_pairs() {
    if (length <= 1)
        return;

    for (Node<T>* curr = head; curr != nullptr && curr->next != nullptr; curr = curr->next->next)
        swap(curr->data, curr->next->data);
}

// ─── Debug ────────────────────────────────────────────────────────────────────

template <typename T>
void LinkedList<T>::debug_print_addresses() const {
    for (Node<T>* curr = head; curr; curr = curr->next)
        cout << curr << "," << curr->data << "\t";
    cout << "\n";
}

template <typename T>
void LinkedList<T>::debug_print_node(Node<T>* node, bool is_separate) const {
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
    for (auto node : debug_data)
        debug_print_node(node);
    cout << "************\n" << flush;
}

template <typename T>
string LinkedList<T>::debug_to_string() const {
    if (length == 0) return "";

    ostringstream oss;
    for (Node<T>* curr = head; curr; curr = curr->next) {
        oss << curr->data;
        if (curr->next) oss << " ";
    }
    return oss.str();
}

template <typename T>
void LinkedList<T>::debug_verify_integrity() const {
    if (length == 0) {
        assert(head == nullptr);
        assert(tail == nullptr);
    } else {
        assert(head != nullptr);
        assert(tail != nullptr);
        if (length == 1) assert(head == tail);
        else             assert(head != tail);
        assert(!tail->next);
    }

    int len = 0;
    for (Node<T>* curr = head; curr; curr = curr->next, len++)
        assert(len < 10000);

    assert(length == len);
    assert(length == (int)debug_data.size());
}
