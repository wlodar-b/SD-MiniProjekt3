#ifndef SINGLYLINKEDLIST_HPP
#define SINGLYLINKEDLIST_HPP

// Węzeł listy dostosowany do słownika (klucz-wartość)
struct Node {
    int key;
    int value;
    Node* next;

    Node(int k, int v) : key(k), value(v), next(nullptr) {}
};

class SinglyLinkedList {
private:
    Node* head;
    Node* tail;
    int size;

public:
    SinglyLinkedList();
    SinglyLinkedList(const SinglyLinkedList& other);
    ~SinglyLinkedList();

    // Umieszcza nową parę (jeśli klucz istnieje, aktualizuje wartość)
    void insert(int key, int value);

    // Usuwa węzeł o podanym kluczu
    bool remove(int key);

    // Zwraca wskaźnik na węzeł, jeśli klucz istnieje (lub nullptr)
    Node* find(int key);

    void clear();
    void display();
    int getSize();
};

#endif // SINGLYLINKEDLIST_HPP