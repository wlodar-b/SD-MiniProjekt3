#include "HashTableList.hpp"
#include <iostream>

HashTableList::HashTableList(int initialCapacity) {
    capacity = initialCapacity;
    size = 0;
    maxLoadFactor = 0.75f;
    
    // Dynamiczna alokacja tablicy wskaźników na listy
    table = new SinglyLinkedList*[capacity];
    for (int i = 0; i < capacity; i++) {
        table[i] = nullptr; // Inicjalizacja pustych kubełków
    }
}

HashTableList::~HashTableList() {
    // Czyszczenie pamięci - usuwamy każdą alokowaną listę
    for (int i = 0; i < capacity; i++) {
        if (table[i] != nullptr) {
            delete table[i];
        }
    }
    // Zwalniamy główną tablicę
    delete[] table; 
}

int HashTableList::hashFunction(int key) {
    // Zabezpieczenie przed ujemnymi kluczami
    int positiveKey = (key < 0) ? -key : key;
    return positiveKey % capacity;
}

void HashTableList::rehash() {
    int oldCapacity = capacity;
    SinglyLinkedList** oldTable = table;

    // Relokacja: Podwajamy pojemność tablicy
    capacity *= 2;
    table = new SinglyLinkedList*[capacity];
    for (int i = 0; i < capacity; i++) {
        table[i] = nullptr;
    }

    size = 0; // Resetujemy licznik, bo insert go zaktualizuje

    // Przemieszanie elementów ze starej tablicy do nowej
    for (int i = 0; i < oldCapacity; i++) {
        if (oldTable[i] != nullptr) {
            // Bezpośredni dostęp do Node dzięki relacji friend
            Node* current = oldTable[i]->head;
            while (current != nullptr) {
                insert(current->key, current->value);
                current = current->next;
            }
            delete oldTable[i]; // Zwalniamy starą listę po skopiowaniu
        }
    }
    delete[] oldTable; // Zwalniamy starą tablicę
}

