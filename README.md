## O projekcie
Celem projektu jest własna implementacja abstrakcyjnej struktury danych – słownika opartego na tablicy mieszającej (ang. *hash table*). Zgodnie z rygorystycznymi wytycznymi, projekt został zrealizowany w czystym języku C++ z całkowitym pominięciem gotowych kontenerów z biblioteki STL (jak `std::vector` czy `std::list` wewnątrz struktur). Wszystkie struktury są alokowane dynamicznie i automatycznie relokują swój rozmiar (rehashing).

W słowniku zarówno klucze, jak i wartości są liczbami całkowitymi.

## Zaimplementowane Warianty (Cel: Ocena 5.0)
Zgodnie z wytycznymi przygotowujemy trzy warianty rozwiązywania kolizji:
- [x] **Wariant 1:** Adresowanie łańcuchowe z użyciem własnej Listy Jednokierunkowej (`HashTableList`).
- [ ] **Wariant 2:** Adresowanie łańcuchowe z użyciem zbalansowanego Drzewa AVL (`HashTableAVL`). *(W trakcie realizacji)*
- [ ] **Wariant 3:** Adresowanie otwarte (np. próbkowanie liniowe). *(Do zrobienia)*

## Struktura Repozytorium (Aktualny Stan)
* `include/IHashTable.hpp` - Czysto wirtualny interfejs dla wszystkich wariantów słownika.
* `include/SinglyLinkedList.hpp` / `src/SinglyLinkedList.cpp` - Własna implementacja kubełka listowego.
* `include/HashTableList.hpp` / `src/HashTableList.cpp` - Wariant 1 (gotowy).
* `include/Utils.hpp` / `src/Utils.cpp` - Generator losowych kluczy i wartości na bazie stałych ziaren (powtarzalność badań).
* `include/TimeMeasurer.hpp` - Moduł do precyzyjnego pomiaru czasu operacji w nanosekundach `[ns]`, tworzący kopię struktury w celu zachowania stałego rozmiaru `n` podczas pomiarów dodawania/usuwania.
* `include/FileManager.hpp` / `src/FileManager.cpp` - Moduł do zapisu/odczytu z plików `.txt` oraz zrzutu wyników badań do pliku `.csv`.

## Co zostało do zrobienia (TO-DO)?
1. Implementacja klasy `AVLTree` oraz spinającej ją klasy `HashTableAVL`.
2. Implementacja Wariantu 3 (Adresowanie otwarte).
3. Implementacja interaktywnego, dwupoziomowego `Menu` oraz głównego pliku `main.cpp` do obsługi i testowania struktur.
4. Przeprowadzenie badań (min. 8 punktów pomiarowych, start od N=5000) i wygenerowanie plików CSV.