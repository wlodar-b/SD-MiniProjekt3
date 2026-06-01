#include "singlyLinkedList.hpp"
#include <iostream>

SinglyLinkedList::SinglyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

SinglyLinkedList::SinglyLinkedList(const SinglyLinkedList& other) : head(nullptr), tail(nullptr), size(0) {
    Node* current = other.head;
    while (current != nullptr) {
        insert(current->key, current->value);
        current = current->next;
    }
}

SinglyLinkedList::~SinglyLinkedList() {
    clear();
}

