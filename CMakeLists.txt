cmake_minimum_required(VERSION 3.10)
project(ccc)

set(CMAKE_CXX_STANDARD 20)

include_directories(
    LinkedList
    Stack
    Vector
)

add_executable(ccc
    main.cpp
    LinkedList/LinkedList.h
    LinkedList/LinkedListBase.h
    LinkedList/OrderedLinkedList.h
    LinkedList/DoublyLinkedList.h
    Stack/DodoStack.h
    Vector/DodoVector.h
)