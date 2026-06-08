#include "HashTableList.hpp"
#include <iostream>

// Konstruktor z domyślną pojemnością
HashTableList::HashTableList(int initialCapacity) {
    capacity = initialCapacity;
    size = 0;
    maxLoadFactor = 0.75f; // domyślny współczynnik obciążenia
    
    // Dynamiczna alokacja tablicy wskaźników na listy
    table = new SinglyLinkedList*[capacity];
    for (int i = 0; i < capacity; i++) {
        table[i] = nullptr; // Inicjalizacja pustych kubełków
    }
}

// Konstruktor kopiujący 
HashTableList::HashTableList(const HashTableList& other) {
    capacity = other.capacity;
    size = other.size;
    maxLoadFactor = other.maxLoadFactor;

    table = new SinglyLinkedList*[capacity];
    for (int i = 0; i < capacity; i++) {
        if (other.table[i] != nullptr) {
            table[i] = new SinglyLinkedList(*other.table[i]); // Kopia listy
        } else {
            table[i] = nullptr;
        }
    }
}

// Destruktor
HashTableList::~HashTableList() {
    for (int i = 0; i < capacity; i++) {
        if (table[i] != nullptr) {
            delete table[i];
        }
    }
    // Zwalniamy główną tablicę
    delete[] table; 
}

// Funkcja mieszająca
int HashTableList::hashFunction(int key) {
    // Zabezpieczenie przed ujemnymi kluczami
    int positiveKey = (key < 0) ? -key : key;
    return positiveKey % capacity;
}

// Funkcja relokująca tablicę i przemieszczająca elementy
void HashTableList::rehash() {
    int oldCapacity = capacity; // Zapamiętujemy starą pojemność
    SinglyLinkedList** oldTable = table; // Zapamiętujemy starą tablicę

    // Relokacja: Podwajamy pojemność tablicy
    capacity *= 2; // Podwajamy pojemność

    // Alokujemy nowa dwukrotniie wieksza tablice i wypelniamy nullptr
    table = new SinglyLinkedList*[capacity]; 
    for (int i = 0; i < capacity; i++) {
        table[i] = nullptr; 
    }

    size = 0; // Resetujemy licznik, bo insert go zaktualizuje

    // Przemieszanie elementów ze starej tablicy do nowej
    for (int i = 0; i < oldCapacity; i++) {
        if (oldTable[i] != nullptr) {
            // Jesli stary kubelek nie byl pusty to wyciagamy jego glowe i przechodzimy przez wszystkie wezly
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

// Dodanie lub aktualizacja pary 
void HashTableList::insert(int key, int value) {
    // Sprawdzenie, czy potrzebna jest relokacja
    if ((float)(size + 1) / capacity > maxLoadFactor) {
        rehash();
    }

    int index = hashFunction(key);

    // Dynamiczna alokacja listy dla danego kubełka 
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

// Usunięcie pary
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