#include <iostream>
#include "LinkedListBase.h"
#include "LinkedList.h"
#include "OrderedLinkedList.h"
#include "DoublyLinkedList.h"
using namespace std;

int main() {

    // ── LinkedList
    cout << "=== LinkedList ===\n";
    LinkedList<int> ll;
    ll.push_back(10);
    ll.push_back(20);
    ll.push_back(30);
    ll.push_front(5);
    ll.insert_at(3, 15);
    ll.print();                  // 5 10 15 20 30

    ll.pop_front();
    ll.pop_back();
    ll.remove_value(15);
    ll.print();                  // 10 20

    ll.push_back(1);
    ll.push_back(99);
    ll.push_back(1);
    ll.remove_min();
    ll.remove_all_occurrences(1);
    ll.print();                  // 10 20 99

    cout << "front: " << ll.front() << "\n";
    cout << "back:  " << ll.back()  << "\n";
    cout << "size:  " << ll.size()  << "\n";
    cout << "search(20): " << ll.search(20) << "\n";

    LinkedList<int> ll2;
    ll2.push_back(4); ll2.push_back(3); ll2.push_back(2); ll2.push_back(1);
    ll2.reverse();
    ll2.print();                 // 1 2 3 4

    ll2.swap_pairs();
    ll2.print();                 // 2 1 4 3

    ll2.rotate();
    ll2.print();                 // 1 4 3 2

    LinkedList<int> half;
    ll2.divide_mid(half);
    cout << "left:  "; ll2.print();
    cout << "right: "; half.print();

    cout << "reversed: "; ll2.reverse_print();

    // ── OrderedLinkedList
    cout << "\n=== OrderedLinkedList ===\n";
    OrderedLinkedList<int> ol;
    ol.insert(40);
    ol.insert(10);
    ol.insert(30);
    ol.insert(20);
    ol.insert(50);
    ol.print();                  // 10 20 30 40 50

    cout << "min: " << ol.min() << "\n";
    cout << "max: " << ol.max() << "\n";
    cout << "search(30): " << ol.search(30) << "\n";
    cout << "search(99): " << ol.search(99) << "\n";

    ol.remove_min();
    ol.remove_max();
    ol.print();                  // 20 30 40

    ol.remove_value(30);
    ol.print();                  // 20 40

    OrderedLinkedList<int> ol2;
    ol2.insert(25); ol2.insert(35); ol2.insert(45);
    ol.merge(ol2);
    ol.print();                  // 20 25 35 40 45

    ol.remove_all_occurrences(35);
    ol.print();                  // 20 25 40 45

    cout << "is_sorted: " << ol.is_sorted() << "\n";
    cout << "reversed:  "; ol.reverse_print();

    // ── DoublyLinkedList
    cout << "\n=== DoublyLinkedList ===\n";
    DoublyLinkedList<int> dl;
    dl.push_back(10);
    dl.push_back(20);
    dl.push_back(30);
    dl.push_front(5);
    dl.insert_at(3, 15);
    dl.print();                  // 5 10 15 20 30
    dl.print_backward();         // 30 20 15 10 5

    dl.pop_front();
    dl.pop_back();
    dl.print();                  // 10 15 20

    dl.remove_value(15);
    dl.print();                  // 10 20

    dl.push_back(1); dl.push_back(5); dl.push_back(1);
    dl.remove_min();
    dl.remove_all_occurrences(1);
    dl.print();                  // 10 20 5

    cout << "front: " << dl.front() << "\n";
    cout << "back:  " << dl.back()  << "\n";
    cout << "size:  " << dl.size()  << "\n";

    DoublyLinkedList<int> dl2;
    dl2.push_back(1); dl2.push_back(2); dl2.push_back(3); dl2.push_back(4);

    dl2.swap_pairs();
    dl2.print();                 // 2 1 4 3

    dl2.rotate();
    dl2.print();                 // 1 4 3 2

    dl2.rotate_backward();
    dl2.print();                 // 2 1 4 3

    dl2.reverse();
    dl2.print();                 // 3 4 1 2

    DoublyLinkedList<int> half2;
    dl2.divide_mid(half2);
    cout << "left:  "; dl2.print();
    cout << "right: "; half2.print();

    DoublyLinkedList<int> copy = dl2;
    cout << "equal: " << (dl2 == copy) << "\n";

    for (int val : dl2) cout << val << " ";
    cout << "\n";

    return 0;
}