#ifndef HASHTABLELIST_HPP
#define HASHTABLELIST_HPP

#include "IHashTable.hpp"
#include "SinglyLinkedList.hpp"

class HashTableList : public IHashTable {
private:
    SinglyLinkedList** table; // Tablica wskaźników na listy (kubełki) 
    int capacity; // Rozmiar tablicy 
    int size; // Liczba elementów w tablicy
    float maxLoadFactor; // Maksymalny współczynnik obciążenia

    // funkcja mieszająca (zwraca indeks kubełka)
    int hashFunction(int key);

    // funkcja relokująca tablicę i przemieszująca elementy
    void rehash();

public:
    HashTableList(int initialCapacity = 16); // Konstruktor z domyślną pojemnością
    HashTableList(const HashTableList& other); // Konstruktor kopiujący
    ~HashTableList() override; // Destruktor

    // Implementacja metod z interfejsu IHashTable
    void insert(int key, int value) override;
    void remove(int key) override;
    bool find(int key, int& out_value) override;
    void display() override;
};

#endif // HASHTABLELIST_HPP