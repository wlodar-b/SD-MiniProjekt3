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

// Konstruktor kopiujący - głęboka kopia (każdy kubełek-lista kopiowany osobno).
// Dzięki temu kopiowanie tablicy (np. w TimeMeasurer) nie powoduje podwójnego zwalniania pamięci.
HashTableList::HashTableList(const HashTableList& other) {
    capacity = other.capacity;
    size = other.size;
    maxLoadFactor = other.maxLoadFactor;

    table = new SinglyLinkedList*[capacity];
    for (int i = 0; i < capacity; i++) {
        if (other.table[i] != nullptr) {
            table[i] = new SinglyLinkedList(*other.table[i]); // Kopia listy (konstruktor kopiujący SinglyLinkedList)
        } else {
            table[i] = nullptr;
        }
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

void HashTableList::insert(int key, int value) {
    // Sprawdzenie, czy potrzebna jest relokacja
    if ((float)(size + 1) / capacity > maxLoadFactor) {
        rehash();
    }

    int index = hashFunction(key);

    // Dynamiczna alokacja listy dla danego kubełka (lazy initialization)
    if (table[index] == nullptr) {
        table[index] = new SinglyLinkedList();
    }

    // Zwiększamy rozmiar słownika tylko wtedy, gdy to nowy klucz
    if (table[index]->find(key) == nullptr) {
        size++; 
    }

    // Dodanie lub aktualizacja wartości w liście
    table[index]->insert(key, value);
}

void HashTableList::remove(int key) {
    int index = hashFunction(key);

    if (table[index] != nullptr) {
        bool removed = table[index]->remove(key);
        if (removed) {
            size--;
            
            // Opcjonalne zwalnianie pamięci: usuwamy pustą listę
            if (table[index]->getSize() == 0) {
                delete table[index];
                table[index] = nullptr;
            }
        }
    }
}

bool HashTableList::find(int key, int& out_value) {
    int index = hashFunction(key);

    if (table[index] == nullptr) {
        return false;
    }

    Node* node = table[index]->find(key);
    if (node != nullptr) {
        out_value = node->value; // Zwracamy wartość przez referencję
        return true;
    }

    return false;
}

void HashTableList::display() {
    std::cout << "--- TABLICA MIESZAJACA (KUBELKI Z LISTA) ---\n";
    std::cout << "Elementow: " << size << " | Pojemnosc tablicy: " << capacity << "\n";
    for (int i = 0; i < capacity; i++) {
        if (table[i] != nullptr) {
            std::cout << "Kubelek [" << i << "]: ";
            table[i]->display();
        }
    }
    std::cout << "--------------------------------------------\n";
}