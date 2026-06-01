#ifndef IHASHTABLE_HPP
#define IHASHTABLE_HPP

class IHashTable {
    public:
        virtual ~IHashTable() = default;

        // Umieszcza nową parę klucz-wartość w słowniku
        virtual void instert(int key, int value) = 0;

        // Usuwa parę klucz-wartość
        virtual void remove(int key) = 0;

        // Szuka wartości po kluczu
        virtual int find(int key, int& out_value) = 0;

        // Wyświetla zawartość struktury
        virtual void display() = 0;
};

#endif // IHASHTABLE_HPP