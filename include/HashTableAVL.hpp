#ifndef HASHTABLEAVL_HPP
#define HASHTABLEAVL_HPP

#include "IHashTable.hpp"

class AVLTree {
    friend class HashTableAVL; // Pozwala tablicy mieszającej przejść po węzłach przy rehashingu
private:
    struct Node {
        int key;
        int value;
        Node* left_child;
        Node* right_child;
        int node_height; // Wysokość poddrzewa (liść ma wysokość 1)

        Node(int k, int v);
    };

    Node* root;
    int size; // Liczba par przechowywanych w drzewie

    // --- Małe funkcje pomocnicze ---
    int height(Node* node);                 // Wysokość węzła (pusty = 0)
    int balanceFactor(Node* node);          // Różnica wysokości lewego i prawego poddrzewa
    void updateHeight(Node* node);          // Przelicza wysokość na podstawie dzieci

    Node* rotateRight(Node* y);
    Node* rotateLeft(Node* x);
    Node* balance(Node* node);              // Balansuje węzeł, zwraca nowy korzeń poddrzewa

    // --- Rekurencyjne operacje na drzewie ---
    Node* insertNode(Node* node, int key, int value);
    Node* findMin(Node* node);              // Węzeł o najmniejszym kluczu w poddrzewie
    Node* removeNode(Node* node, int key);
    void clearNode(Node* node);             // Usuwa całe poddrzewo
    Node* copyNode(Node* node);             // Tworzy głęboką kopię poddrzewa
    void displayNode(Node* node);           // Wypisuje pary rosnąco po kluczu

public:
    AVLTree();
    AVLTree(const AVLTree& other);          // Konstruktor kopiujący (głęboka kopia)
    ~AVLTree();

    // Wstawia parę (lub aktualizuje wartość, gdy klucz już istnieje)
    void insert(int key, int value);

    // Usuwa parę o podanym kluczu
    void remove(int key);

    // Szuka wartości po kluczu. Zwraca true i ustawia out_value, gdy klucz istnieje.
    bool find(int key, int& out_value);

    void clear();
    void display();
    int getSize();
};



// Tablica mieszająca z adresowaniem łańcuchowym, gdzie każdy kubełek to drzewo AVL.
// Wariant 2 projektu. Dzięki drzewom AVL nawet przy wielu kolizjach w jednym
// kubełku wyszukiwanie pozostaje szybkie - O(log k), gdzie k to rozmiar kubełka.
class HashTableAVL : public IHashTable {
private:
    AVLTree** table;          // Dynamiczna tablica wskaźników na drzewa AVL
    int capacity;             // Aktualna pojemność tablicy
    int size;                 // Aktualna liczba wszystkich par w słowniku
    float maxLoadFactor;      // Próg wypełnienia, po którym powiększamy tablicę

    // Prywatna funkcja mieszająca (zwraca indeks kubełka)
    int hashFunction(int key);

    // Prywatna funkcja relokująca tablicę i przemieszująca elementy (rehashing)
    void rehash();

    // Pomocnicza: wstawia wszystkie pary z drzewa do bieżącej tablicy (używana w rehash)
    void reinsertTree(AVLTree::Node* node);

public:
    HashTableAVL(int initialCapacity = 16);
    HashTableAVL(const HashTableAVL& other); // Konstruktor kopiujący (głęboka kopia)
    ~HashTableAVL() override;

    // Implementacja metod z interfejsu IHashTable
    void insert(int key, int value) override;
    void remove(int key) override;
    bool find(int key, int& out_value) override;
    void display() override;
};

#endif // HASHTABLEAVL_HPP
