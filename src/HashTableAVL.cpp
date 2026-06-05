#include "HashTableAVL.hpp"
#include <iostream>
#include <algorithm> // std::max

// Nowy węzeł jest zawsze liściem (wysokość 1)
AVLTree::Node::Node(int k, int v) : key(k), value(v),
                                    left_child(nullptr), right_child(nullptr),
                                    node_height(1) {}

AVLTree::AVLTree() : root(nullptr), size(0) {}

// Konstruktor kopiujący - potrzebny przy kopiowaniu tablicy mieszającej
AVLTree::AVLTree(const AVLTree& other) : root(nullptr), size(other.size) {
    root = copyNode(other.root);
}

AVLTree::~AVLTree() {
    clear();
}

// --- Małe funkcje pomocnicze ---

// Bezpieczne pobranie wysokości (pusty węzeł ma wysokość 0)
int AVLTree::height(Node* node) {
    if (node == nullptr) return 0;
    return node->node_height;
}

// Współczynnik zbalansowania: wysokość lewego - wysokość prawego poddrzewa.
// Dla zbalansowanego węzła mieści się w przedziale od -1 do 1.
int AVLTree::balanceFactor(Node* node) {
    if (node == nullptr) return 0;
    return height(node->left_child) - height(node->right_child);
}

// Aktualizuje wysokość węzła na podstawie jego dzieci
void AVLTree::updateHeight(Node* node) {
    node->node_height = 1 + std::max(height(node->left_child), height(node->right_child));
}


// Rotacja w prawo wokół węzła y (gdy przeważa lewa strona)
AVLTree::Node* AVLTree::rotateRight(Node* y) {
    Node* x = y->left_child;
    Node* T2 = x->right_child;

    x->right_child = y;
    y->left_child = T2;

    updateHeight(y);
    updateHeight(x);

    return x; // Nowy korzeń tego poddrzewa
}

// Rotacja w lewo wokół węzła x (gdy przeważa prawa strona)
AVLTree::Node* AVLTree::rotateLeft(Node* x) {
    Node* y = x->right_child;
    Node* T2 = y->left_child;

    y->left_child = x;
    x->right_child = T2;

    updateHeight(x);
    updateHeight(y);

    return y; // Nowy korzeń tego poddrzewa
}

// Sprawdza zbalansowanie węzła i w razie potrzeby wykonuje rotacje.
// Zwraca  korzeń poddrzewa.
AVLTree::Node* AVLTree::balance(Node* node) {
    updateHeight(node);
    int bf = balanceFactor(node);

    // Przeważa lewa strona, lewe dziecko też w lewo
    if (bf > 1 && balanceFactor(node->left_child) >= 0) {
        return rotateRight(node);
    }

    // Przeważa lewa strona, ale lewe dziecko w prawo
    if (bf > 1 && balanceFactor(node->left_child) < 0) {
        node->left_child = rotateLeft(node->left_child);
        return rotateRight(node);
    }

    // Przeważa prawa strona, prawe dziecko też w prawo
    if (bf < -1 && balanceFactor(node->right_child) <= 0) {
        return rotateLeft(node);
    }

    // Przeważa prawa strona, ale prawe dziecko w lewo
    if (bf < -1 && balanceFactor(node->right_child) > 0) {
        node->right_child = rotateRight(node->right_child);
        return rotateLeft(node);
    }

    return node; // Węzeł jest już zbalansowany
}

// --- Rekurencyjne operacje na drzewie ---

// Wstawia parę klucz-wartość. Jeśli klucz już istnieje, aktualizuje wartość.
AVLTree::Node* AVLTree::insertNode(Node* node, int key, int value) {
    // Doszliśmy do pustego miejsca - tu tworzymy nowy węzeł
    if (node == nullptr) {
        size++;
        return new Node(key, value);
    }

    if (key < node->key) {
        node->left_child = insertNode(node->left_child, key, value);
    } else if (key > node->key) {
        node->right_child = insertNode(node->right_child, key, value);
    } else {
        node->value = value; // Klucz istnieje - tylko aktualizacja wartości
        return node;
    }

    // Po wstawieniu balansujemy w drodze powrotnej w górę
    return balance(node);
}

// Zwraca węzeł o najmniejszym kluczu w poddrzewie (skrajnie lewy)
AVLTree::Node* AVLTree::findMin(Node* node) {
    while (node->left_child != nullptr) {
        node = node->left_child;
    }
    return node;
}

// Usuwa węzeł o podanym kluczu (jeśli istnieje)
AVLTree::Node* AVLTree::removeNode(Node* node, int key) {
    if (node == nullptr) return nullptr; // Klucza nie ma w drzewie

    if (key < node->key) {
        node->left_child = removeNode(node->left_child, key);
    } else if (key > node->key) {
        node->right_child = removeNode(node->right_child, key);
    } else {
        // Znaleziono węzeł do usunięcia

        // Brak dziecka lub tylko jedno dziecko
        if (node->left_child == nullptr || node->right_child == nullptr) {
            Node* child = (node->left_child != nullptr) ? node->left_child
                                                        : node->right_child;
            delete node;
            size--;
            return child; // Podpinamy jedyne dziecko (lub nullptr) w miejsce węzła
        }

        // Dwoje dzieci: zastępujemy węzeł najmniejszym kluczem z prawego
        // poddrzewa, a potem usuwamy tamten węzeł z prawego poddrzewa.
        Node* successor = findMin(node->right_child);
        node->key = successor->key;
        node->value = successor->value;
        node->right_child = removeNode(node->right_child, successor->key);
    }

    // Po usunięciu balansujemy w drodze powrotnej w górę
    return balance(node);
}

// Rekurencyjne usuwanie całego drzewa
void AVLTree::clearNode(Node* node) {
    if (node == nullptr) return;
    clearNode(node->left_child);
    clearNode(node->right_child);
    delete node;
}

// Tworzy głęboką kopię drzewa
AVLTree::Node* AVLTree::copyNode(Node* node) {
    if (node == nullptr) return nullptr;
    Node* copy = new Node(node->key, node->value);
    copy->node_height = node->node_height;
    copy->left_child = copyNode(node->left_child);
    copy->right_child = copyNode(node->right_child);
    return copy;
}

// Wypisanie par w kolejności rosnących kluczy
void AVLTree::displayNode(Node* node) {
    if (node == nullptr) return;
    displayNode(node->left_child);
    std::cout << "[" << node->key << ":" << node->value << "] ";
    displayNode(node->right_child);
}

// --- Metody publiczne ---

// Wstawia parę (lub aktualizuje wartość, gdy klucz już istnieje)
void AVLTree::insert(int key, int value) {
    root = insertNode(root, key, value);
}

// Usuwa parę o podanym kluczu
void AVLTree::remove(int key) {
    root = removeNode(root, key);
}

// Szuka wartości po kluczu. Zwraca true i ustawia out_value, gdy klucz istnieje.
bool AVLTree::find(int key, int& out_value) {
    Node* current = root;
    while (current != nullptr) {
        if (key < current->key) {
            current = current->left_child;
        } else if (key > current->key) {
            current = current->right_child;
        } else {
            out_value = current->value; // Znaleziono
            return true;
        }
    }
    return false; // Nie znaleziono klucza
}

void AVLTree::clear() {
    clearNode(root);
    root = nullptr;
    size = 0;
}

// Wypisuje pary posortowane rosnąco po kluczu
void AVLTree::display() {
    displayNode(root);
    std::cout << "\n";
}

int AVLTree::getSize() {
    return size;
}
