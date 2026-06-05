#ifndef HASHTABLELINEAR_HPP
#define HASHTABLELINEAR_HPP

#include "IHashTable.hpp"

// Stany dla pojedynczego kubełka w adresowaniu otwartym
enum EntryState {
    EMPTY,
    OCCUPIED,
    DELETED
};

// Struktura pojedynczego wpisu w tablicy
struct HashEntry {
    int key;
    int value;
    EntryState state;

    HashEntry() : key(0), value(0), state(EMPTY) {}
};

class HashTableLinear : public IHashTable {
private:
    HashEntry* table;         // Dynamiczna tablica przechowująca elementy bezpośrednio
    int capacity;             // Aktualna pojemność tablicy
    int size;                 // Liczba zajętych elementów (OCCUPIED)
    int deletedCount;         // Liczba usuniętych elementów (DELETED) - potrzebne do współczynnika wypełnienia
    float maxLoadFactor;      // Próg wypełnienia (zalecany niższy dla adresowania otwartego, np. 0.5 - 0.7)

    int hashFunction(int key);
    void rehash();

public:
    HashTableLinear(int initialCapacity = 16);
    ~HashTableLinear() override;

    void insert(int key, int value) override;
    void remove(int key) override;
    bool find(int key, int& out_value) override;
    void display() override;
};

#endif // HASHTABLELINEAR_HPP