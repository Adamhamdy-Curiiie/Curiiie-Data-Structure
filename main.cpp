#include <iostream>
#include "Node.h"
#include "DodoStack.h"
#include "DodoVector.h"
using namespace std;

int main() {
    LinkedList<int> list;
    list.push_front(5);
    list.push_front(6);
    list.push_front(7);
    list.print();
    return 0;
}
