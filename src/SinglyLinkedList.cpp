#include "SinglyLinkedList.hpp"
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

void SinglyLinkedList::insert(int key, int value) {
    // Sprawdzamy, czy klucz już istnieje
    Node* existingNode = find(key);
    if (existingNode != nullptr) {
        existingNode->value = value; // Aktualizacja wartości
        return;
    }

    // Dodawanie na początek - O(1) (najlepsze dla tablic mieszających)
    Node* newNode = new Node(key, value);
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head = newNode;
    }
    size++;
}

bool SinglyLinkedList::remove(int key) {
    if (head == nullptr) return false;

    // Przypadek gdy usuwamy z początku (head)
    if (head->key == key) {
        Node* temp = head;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        }
        delete temp;
        size--;
        return true;
    }

    // Szukanie klucza w głębi listy
    Node* current = head;
    while (current->next != nullptr && current->next->key != key) {
        current = current->next;
    }

    if (current->next != nullptr) {
        Node* toDelete = current->next;
        current->next = toDelete->next;
        if (toDelete == tail) {
            tail = current;
        }
        delete toDelete;
        size--;
        return true;
    }

    return false; // Nie znaleziono klucza
}

Node* SinglyLinkedList::find(int key) {
    Node* current = head;
    while (current != nullptr) {
        if (current->key == key) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

void SinglyLinkedList::clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
    head = tail = nullptr;
    size = 0;
}

void SinglyLinkedList::display() {
    Node* current = head;
    while (current != nullptr) {
        std::cout << "[" << current->key << ":" << current->value << "] -> ";
        current = current->next;
    }
    std::cout << "nullptr\n";
}

int SinglyLinkedList::getSize() {
    return size;
}