#include "HashTableLinear.hpp"
#include <iostream>

// Konstruktor z domyślną pojemnością
HashTableLinear::HashTableLinear(int initialCapacity) {
    capacity = initialCapacity; // Ustawiamy początkową pojemność
    size = 0; // brak elementow na poczatku
    deletedCount = 0; // brak nagrobków na poczatku
    maxLoadFactor = 0.5f; // 
    
    table = new HashEntry[capacity]; // Alokacja bezpośrednio ciągłego bloku pamięci
}

// Konstruktor kopiujący
// HashEntry to prosta struktura, więc wystarczy skopiować elementy jeden po drugim
HashTableLinear::HashTableLinear(const HashTableLinear& other) {
    capacity = other.capacity;
    size = other.size;
    deletedCount = other.deletedCount;
    maxLoadFactor = other.maxLoadFactor;

    table = new HashEntry[capacity];
    for (int i = 0; i < capacity; i++) {
        table[i] = other.table[i];
    }
}

HashTableLinear::~HashTableLinear() {
    delete[] table;
}

int HashTableLinear::hashFunction(int key) {
    int positiveKey = (key < 0) ? -key : key;
    return positiveKey % capacity;
}

// Funkcja relokująca tablicę
void HashTableLinear::rehash() {
    int oldCapacity = capacity;
    HashEntry* oldTable = table;

    // Podwajamy pojemność
    capacity *= 2;
    table = new HashEntry[capacity];
    size = 0;
    deletedCount = 0; // Nagrobki nie są przenoszone do nowej tablicy

    // Przepisujemy tylko istniejące elementy 
    for (int i = 0; i < oldCapacity; i++) {
        if (oldTable[i].state == OCCUPIED) {
            insert(oldTable[i].key, oldTable[i].value);
        }
    }

    delete[] oldTable;
}

// Dodanie lub aktualizacja pary
void HashTableLinear::insert(int key, int value) {
    if ((float)(size + deletedCount + 1) / capacity > maxLoadFactor) {
        rehash();
    }

    int index = hashFunction(key);
    int firstDeleted = -1;

    // Próbkowanie liniowe w poszukiwaniu klucza lub wolnego miejsca
    while (table[index].state != EMPTY) {
        if (table[index].state == OCCUPIED && table[index].key == key) {
            table[index].value = value;
            return;
        }
        if (table[index].state == DELETED && firstDeleted == -1) {
            firstDeleted = index;
        }
        index = (index + 1) % capacity; // Przejście do następnego indeksu
    }

    if (firstDeleted != -1) {
        index = firstDeleted;
        deletedCount--; // Zastępujemy nagrobek nowym elementem
    }

    table[index].key = key;
    table[index].value = value;
    table[index].state = OCCUPIED;
    size++;
}

// Usunięcie pary
void HashTableLinear::remove(int key) {
    int index = hashFunction(key);

    while (table[index].state != EMPTY) {
        if (table[index].state == OCCUPIED && table[index].key == key) {
            table[index].state = DELETED; // Zaznaczamy jako usunięty 
            size--;
            deletedCount++;
            return;
        }
        index = (index + 1) % capacity;
    }
}

bool HashTableLinear::find(int key, int& out_value) {
    int index = hashFunction(key);

    while (table[index].state != EMPTY) {
        if (table[index].state == OCCUPIED && table[index].key == key) {
            out_value = table[index].value;
            return true;
        }
        index = (index + 1) % capacity;
    }

    return false;
}

void HashTableLinear::display() {
    std::cout << "--- TABLICA MIESZAJACA (ADRESOWANIE OTWARTE) ---\n";
    std::cout << "Zajete: " << size << " | Usuniete: " << deletedCount << " | Pojemnosc: " << capacity << "\n";
    for (int i = 0; i < capacity; i++) {
        std::cout << "Indeks [" << i << "]: ";
        if (table[i].state == OCCUPIED) {
            std::cout << "[" << table[i].key << ":" << table[i].value << "]\n";
        } else if (table[i].state == DELETED) {
            std::cout << "[DELETED]\n";
        } else {
            std::cout << "EMPTY\n";
        }
    }
    std::cout << "------------------------------------------------\n";
}