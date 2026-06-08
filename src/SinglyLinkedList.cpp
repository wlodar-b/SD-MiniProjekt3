#include "SinglyLinkedList.hpp"
#include <iostream>

// Implementacja metod klasy SinglyLinkedList
SinglyLinkedList::SinglyLinkedList() : head(nullptr), tail(nullptr), size(0) {}


// Konstruktor kopiujący - tworzy głęboką kopię listy 
SinglyLinkedList::SinglyLinkedList(const SinglyLinkedList& other) : head(nullptr), tail(nullptr), size(0) {
    Node* current = other.head;
    while (current != nullptr) {
        insert(current->key, current->value);
        current = current->next;
    }
}

// Destruktor
SinglyLinkedList::~SinglyLinkedList() {
    clear();
}

// Metoda insert - dodaje nowa pare lub aktualizuje istniejąca
void SinglyLinkedList::insert(int key, int value) {
    // Sprawdzamy, czy klucz już istnieje
    Node* existingNode = find(key);
    if (existingNode != nullptr) {
        existingNode->value = value; // Aktualizacja wartości
        return;
    }

    // Dodawanie na początek - O(1) 
    Node* newNode = new Node(key, value);
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head = newNode;
    }
    size++;
}

// Metoda remove - usuwa węzeł o podanym kluczu
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

// Metoda find - zwraca wskaźnik na węzeł o podanym kluczu lub nullptr
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

// Metoda clear - usuwa wszystkie elementy z listy
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

// Metoda display - wyświetla zawartość listy
void SinglyLinkedList::display() {
    Node* current = head;
    while (current != nullptr) {
        std::cout << "[" << current->key << ":" << current->value << "] -> ";
        current = current->next;
    }
    std::cout << "nullptr\n";
}

// Metoda getSize - zwraca liczbę elementów w liście
int SinglyLinkedList::getSize() {
    return size;
}