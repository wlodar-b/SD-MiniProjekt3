#ifndef HASHTABLEAVL_HPP
#define HASHTABLEAVL_HPP

class AVLTree {
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

#endif 
