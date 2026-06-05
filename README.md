## O projekcie
Celem projektu jest własna implementacja abstrakcyjnej struktury danych – słownika opartego na tablicy mieszającej (ang. *hash table*). Zgodnie z rygorystycznymi wytycznymi, projekt został zrealizowany w czystym języku C++ z całkowitym pominięciem gotowych kontenerów z biblioteki STL (jak `std::vector` czy `std::list` wewnątrz struktur). Wszystkie struktury są alokowane dynamicznie i automatycznie relokują swój rozmiar.

W słowniku zarówno klucze, jak i wartości są liczbami całkowitymi.

## Zaimplementowane Warianty (Cel: Ocena 5.0)
Zgodnie z wytycznymi przygotowujemy trzy warianty rozwiązywania kolizji:
- [x] **Wariant 1:** Adresowanie łańcuchowe z użyciem własnej Listy Jednokierunkowej (`HashTableList`).
- [ ] **Wariant 2:** Adresowanie łańcuchowe z użyciem zbalansowanego Drzewa AVL (`HashTableAVL`). 
- [x] **Wariant 3:** Adresowanie otwarte z próbkowaniem liniowym - Linear Probing (`HashTableLinear`). 

## Struktura Repozytorium (Aktualny Stan)
* `include/IHashTable.hpp` - Czysto wirtualny interfejs dla wszystkich wariantów słownika.
* `include/SinglyLinkedList.hpp` / `src/SinglyLinkedList.cpp` - Własna implementacja kubełka listowego.
* `include/HashTableList.hpp` / `src/HashTableList.cpp` - Wariant 1 (gotowy).
* `include/HashTableLinear.hpp` / `src/HashTableLinear.cpp` - Wariant 3 (gotowy).
* `include/Utils.hpp` / `src/Utils.cpp` - Generator losowych kluczy i wartości na bazie stałych ziaren (powtarzalność badań).
* `include/TimeMeasurer.hpp` - Moduł do precyzyjnego pomiaru czasu operacji w nanosekundach `[ns]`, tworzący kopię struktury przed pomiarem.
* `include/FileManager.hpp` / `src/FileManager.cpp` - Moduł do generowania danych wejściowych (`.txt`) oraz zapisywania wyników badań do pliku `.csv`.

## Zadania do wykonania (TO-DO dla: [Imię Kolegi]):
1. **Wariant 2 (Drzewo AVL):** Napisać własną klasę `AVLTree` oraz spinającą ją z interfejsem klasę `HashTableAVL`.
2. **Menu Główne:** Zaimplementować od zera `Menu.hpp` i `Menu.cpp` obsługujące interaktywnie wszystkie 3 struktury (dodawanie, usuwanie, wyszukiwanie, wyświetlanie).
3. **Badania i Zapis (CSV):** Podpiąć do Menu moduł `TimeMeasurer` oraz funkcję `FileManager::saveToCSV`. Badania należy wykonać zgodnie z wytycznymi (zawsze uśredniać wyniki z prób na różnych seedach, minimum 8 punktów pomiarowych, start rozmiaru od N=5000).
4. **Main:** Spiąć wszystko w `main.cpp`.