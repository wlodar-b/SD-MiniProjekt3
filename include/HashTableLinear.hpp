#ifndef HASHTABLELINEAR_HPP
#define HASHTABLELINEAR_HPP

#include "IHashTable.hpp"

// Stany dla pojedynczego kubełka w adresowaniu otwartym
enum EntryState {
    EMPTY, // komorka jesty pusta i nigdy nic w niej nie bylo 
    OCCUPIED, // komórka przechowuje aktualna pare
    DELETED // nagrobek, element tu byl ale zostal usuniet
};

// Struktura pojedynczego wpisu w tablicy
struct HashEntry {
    int key; // klucz
    int value; // wartosc
    EntryState state; // stan wpisu

    HashEntry() : key(0), value(0), state(EMPTY) {} // Konstruktor domyślny - inicjalizuje jako puste
};

// Implementacja tablicy mieszającej
class HashTableLinear : public IHashTable {
private:
    HashEntry* table; // Tablica dynamiczna przechowująca wpisy
    int capacity; // Rozmiar tablicy
    int size; // Liczba przechowywanych elementów
    int deletedCount; // Liczba usuniętych elementów (nagrobków)
    float maxLoadFactor; // Maksymalny współczynnik obciążenia

    int hashFunction(int key); // Funkcja mieszająca
    void rehash();

public:
    HashTableLinear(int initialCapacity = 16);
    HashTableLinear(const HashTableLinear& other); // Konstruktor kopiujący 
    ~HashTableLinear() override;

    void insert(int key, int value) override;
    void remove(int key) override;
    bool find(int key, int& out_value) override;
    void display() override;
};

#endif // HASHTABLELINEAR_HPP