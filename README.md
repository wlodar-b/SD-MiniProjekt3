## O projekcie
Celem projektu była samodzielna implementacja abstrakcyjnej struktury danych – **słownika operującego na parach klucz-wartość**, opartego na tablicy mieszającej (ang. *hash table*). 

Zgodnie z rygorystycznymi wytycznymi, projekt został zrealizowany w czystym języku C++ z **całkowitym pominięciem kontenerów z biblioteki STL** (takich jak `std::vector`, `std::list` czy `std::map`) w architekturze samych struktur. Zarówno klucze, jak i wartości są 4-bajtowymi liczbami całkowitymi. Wszystkie struktury i ich węzły są alokowane w pełni dynamicznie, a tablice automatycznie relokują swój rozmiar po przekroczeniu ustalonego współczynnika wypełnienia (*Load Factor*).

## Zaimplementowane Warianty (Ocena 5.0)
W ramach projektu przygotowano trzy różne warianty rozwiązywania problemu kolizji. Wszystkie dziedziczą po wspólnym, czysto wirtualnym interfejsie `IHashTable`.

- [x] **Wariant 1: Adresowanie łańcuchowe z Listą Jednokierunkową (`HashTableList`)** Klasyczne podejście, w którym każdy kubełek tablicy zawiera wskaźnik na własną implementację dynamicznej listy jednokierunkowej.
- [x] **Wariant 2: Adresowanie łańcuchowe z Drzewem AVL (`HashTableAVL`)** Zaawansowane podejście, w którym listy zastąpiono zbalansowanymi drzewami poszukiwań binarnych (AVL). Gwarantuje to logarytmiczny czas operacji $O(\log k)$ wewnątrz kubełka nawet przy ekstremalnej liczbie kolizji.
- [x] **Wariant 3: Adresowanie otwarte z próbkowaniem liniowym (`HashTableLinear`)** Podejście bezwskaźnikowe. Elementy przechowywane są bezpośrednio w głównej tablicy. Zaimplementowano mechanizm tzw. "nagrobków" (`DELETED` / Tombstones), niezbędny do poprawnego działania operacji usuwania i wyszukiwania.

## Architektura i Pomiary
Zadbano o wysoką rzetelność przeprowadzanych badań wydajnościowych:
* **Izolacja pomiarów (Stałe N):** Zgodnie z FAQ, przed wykonaniem operacji modyfikujących (`insert`, `remove`) moduł `TimeMeasurer` wykonuje *Głęboką Kopię (Deep Copy)* struktury. Gwarantuje to, że mierzony jest czas operacji wyłącznie dla zadanego, stałego rozmiaru tablicy.
* **Powtarzalność:** Algorytm generujący dane opiera się na 5 stałych ziarnach (seeds) dla generatora `std::mt19937`.
* **Uśrednianie:** Każdy wynik zapisywany do pliku `.csv` jest średnią z 150 prób (5 ziaren x 30 powtórzeń). Czas mierzony jest z wysoką precyzją w nanosekundach `[ns]`.

## Struktura Repozytorium
* `include/` - Pliki nagłówkowe:
  * `IHashTable.hpp` - Interfejs bazowy.
  * `SinglyLinkedList.hpp` / `HashTableList.hpp` - Wariant 1.
  * `HashTableAVL.hpp` - Wariant 2 (Drzewo + Tablica).
  * `HashTableLinear.hpp` - Wariant 3.
  * `TimeMeasurer.hpp` - Narzędzie do pomiaru czasu i zarządzania kopiami struktury.
  * `Utils.hpp` / `FileManager.hpp` - Generowanie danych losowych i eksport do pliku `.csv`.
  * `Menu.hpp` - Moduł głównego menu.
* `src/` - Pliki źródłowe (`.cpp`) odpowiadające nagłówkom oraz główny plik `main.cpp`.

## Kompilacja i Uruchomienie

Projekt nie wymaga żadnych zewnętrznych bibliotek. Ze względu na użycie specyfikatora `constexpr` dla niektórych stałych, wymagany jest standard **C++17**.

**Polecenie kompilacji (z poziomu głównego katalogu projektu):**
```bash
g++ -std=c++17 src/*.cpp -I include -o miniprojekt3