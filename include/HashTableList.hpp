#ifndef HASHTABLELIST_HPP
#define HASHTABLELIST_HPP

#include "IHashTable.hpp"
#include "SinglyLinkedList.hpp"

class HashTableList : public IHashTable {
private:
    SinglyLinkedList** table; // Dynamiczna tablica wskaźników na listy jednokierunkowe
    int capacity;             // Aktualna pojemność tablicy
    int size;                 // Aktualna liczba wszystkich par w słowniku
    float maxLoadFactor;      // Próg wypełnienia, po którym następuje powiększenie tablicy

    // Prywatna funkcja mieszająca (zwraca indeks kubełka)
    int hashFunction(int key);

    // Prywatna funkcja relokująca tablicę i przemieszująca elementy (rehashing)
    void rehash();

public:
    HashTableList(int initialCapacity = 16);
    ~HashTableList() override;

    // Implementacja metod z interfejsu IHashTable
    void insert(int key, int value) override;
    void remove(int key) override;
    bool find(int key, int& out_value) override;
    void display() override;
};

#endif // HASHTABLELIST_HPP