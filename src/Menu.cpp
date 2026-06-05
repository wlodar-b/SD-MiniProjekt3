#include "Menu.hpp"

#include "IHashTable.hpp"
#include "HashTableList.hpp"
#include "HashTableAVL.hpp"
#include "HashTableLinear.hpp"
#include "Utils.hpp"
#include "FileManager.hpp"
#include "TimeMeasurer.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstdio> // std::remove

// --- Parametry badań (zgodnie z wytycznymi) ---
// Każdy wynik to średnia ze 150 pomiarów: 5 różnych seedów x 30 powtórzeń.
static const int SEED_COUNT = 5;
static const int REPEATS_PER_SEED = 30;

// Generuje n par (klucz, wartość) dla zadanego seeda.
// Klucze losowane są z zakresu [0, 10n] - kilkukrotnie większego niż rozmiar struktury,
// co wymusza kolizje. Ten sam seedIndex zawsze daje ten sam zestaw -> powtarzalność badań.
static std::vector<DataPair> generateData(int n, int seedIndex) {
    RandomGenerator rng;
    rng.reset(seedIndex);

    std::vector<DataPair> data;
    data.reserve(n);
    for (int i = 0; i < n; i++) {
        int key = rng.generateKey(n);
        int value = rng.generateValue();
        data.push_back({key, value});
    }
    return data;
}

// Mierzy średni czas operacji insert / find / remove dla danej struktury i rozmiaru n.
//
// Każdy pojedynczy pomiar to JEDNA operacja wykonana na świeżej KOPII struktury rozmiaru n.
// Dzięki kopiom rozmiar n pozostaje stały podczas pomiaru (izolacja), a TimeMeasurer
// kosztowną kopię robi POZA pomiarem czasu - mierzony jest wyłącznie czas operacji.
// Liczba pomiarów = SEED_COUNT (5) x REPEATS_PER_SEED (30) = 150.
template <typename TableType>
static void benchmarkSize(const std::string& name, int n, const std::string& csvFile) {
    long long sumInsert = 0, sumFind = 0, sumRemove = 0;

    for (int seed = 0; seed < SEED_COUNT; seed++) {
        std::vector<DataPair> data = generateData(n, seed);

        // Budujemy strukturę rozmiaru n (wzorzec, który TimeMeasurer będzie kopiował)
        TableType original;
        for (const DataPair& p : data) {
            original.insert(p.key, p.value);
        }

        // Klucze do pojedynczych operacji
        int existingKey = data[n / 2].key; // na pewno obecny w strukturze
        int newKey = 10 * n + 1;           // poza zakresem generacji -> na pewno nieobecny
        int newValue = 1;

        // TimeMeasurer wykonuje REPEATS_PER_SEED prób, każda na świeżej kopii oryginału.
        TimeMeasurer<TableType> measurer(original, REPEATS_PER_SEED);

        // insert - wstawienie nowego klucza do struktury rozmiaru n
        sumInsert += measurer.measure([newKey, newValue](TableType& t) {
            t.insert(newKey, newValue);
        });

        // find - wyszukanie istniejącego klucza
        sumFind += measurer.measure([existingKey](TableType& t) {
            int out_value;
            t.find(existingKey, out_value);
        });

        // remove - usunięcie istniejącego klucza ze struktury rozmiaru n
        sumRemove += measurer.measure([existingKey](TableType& t) {
            t.remove(existingKey);
        });
    }

    // Średnia ze 150 pomiarów (czas pojedynczej operacji w ns)
    long long totalMeasurements = (long long)SEED_COUNT * REPEATS_PER_SEED;
    long long avgInsert = sumInsert / totalMeasurements;
    long long avgFind   = sumFind / totalMeasurements;
    long long avgRemove = sumRemove / totalMeasurements;

    // Zapis do CSV (jeden wiersz na każdą operację)
    FileManager::saveToCSV(csvFile, name, "insert", n, avgInsert);
    FileManager::saveToCSV(csvFile, name, "find",   n, avgFind);
    FileManager::saveToCSV(csvFile, name, "remove", n, avgRemove);

    // Podgląd na konsoli
    std::cout << std::left
              << std::setw(18) << name
              << std::setw(10) << n
              << std::setw(12) << avgInsert
              << std::setw(12) << avgFind
              << std::setw(12) << avgRemove << "\n";
}

// Tworzy wybraną strukturę (zwraca wskaźnik przez wspólny interfejs)
static IHashTable* createStructure(int choice) {
    switch (choice) {
        case 1: return new HashTableList();
        case 2: return new HashTableAVL();
        case 3: return new HashTableLinear();
        default: return nullptr;
    }
}

void Menu::runInteractive() {
    std::cout << "\nWybierz strukture:\n"
              << " 1. HashTableList   (adresowanie lancuchowe - lista)\n"
              << " 2. HashTableAVL    (adresowanie lancuchowe - drzewo AVL)\n"
              << " 3. HashTableLinear (adresowanie otwarte - liniowe)\n"
              << "Wybor: ";

    int choice;
    if (!(std::cin >> choice)) return;

    IHashTable* table = createStructure(choice);
    if (table == nullptr) {
        std::cout << "Nieprawidlowy wybor.\n";
        return;
    }

    int option = -1;
    while (option != 0) {
        std::cout << "\n--- Operacje ---\n"
                  << " 1. Dodaj (klucz wartosc)\n"
                  << " 2. Usun (klucz)\n"
                  << " 3. Szukaj (klucz)\n"
                  << " 4. Wyswietl\n"
                  << " 0. Powrot\n"
                  << "Wybor: ";

        if (!(std::cin >> option)) break;

        if (option == 1) {
            int key, value;
            std::cout << "Podaj klucz i wartosc: ";
            if (std::cin >> key >> value) {
                table->insert(key, value);
                std::cout << "Dodano [" << key << ":" << value << "]\n";
            }
        } else if (option == 2) {
            int key;
            std::cout << "Podaj klucz: ";
            if (std::cin >> key) {
                table->remove(key);
                std::cout << "Usunieto klucz " << key << " (jesli istnial)\n";
            }
        } else if (option == 3) {
            int key, value;
            std::cout << "Podaj klucz: ";
            if (std::cin >> key) {
                if (table->find(key, value)) {
                    std::cout << "Znaleziono wartosc: " << value << "\n";
                } else {
                    std::cout << "Nie znaleziono klucza " << key << "\n";
                }
            }
        } else if (option == 4) {
            table->display();
        }
    }

    delete table;
}

void Menu::runBenchmarks() {
    const std::string csvFile = "wyniki.csv";

    // Usuwamy stary plik, bo saveToCSV tylko dopisuje wiersze
    std::remove(csvFile.c_str());

    // Rozmiary dobrane eksperymentalnie: minimum 8 punktow, start od n >= 5000
    const int sizes[] = {5000, 8000, 10000, 16000, 20000, 40000, 60000, 100000};
    const int sizeCount = sizeof(sizes) / sizeof(sizes[0]);

    std::cout << "\n===== POMIARY WYDAJNOSCI =====\n";
    std::cout << "Sredni czas POJEDYNCZEJ operacji w nanosekundach [ns].\n";
    std::cout << "Srednia ze 150 pomiarow (" << SEED_COUNT << " seedow x "
              << REPEATS_PER_SEED << " powtorzen), operacje mierzone na kopiach (staly rozmiar n).\n\n";

    std::cout << std::left
              << std::setw(18) << "Struktura"
              << std::setw(10) << "N"
              << std::setw(12) << "insert"
              << std::setw(12) << "find"
              << std::setw(12) << "remove" << "\n";
    std::cout << std::string(64, '-') << "\n";

    for (int i = 0; i < sizeCount; i++) {
        int n = sizes[i];
        benchmarkSize<HashTableList>("HashTableList", n, csvFile);
        benchmarkSize<HashTableAVL>("HashTableAVL", n, csvFile);
        benchmarkSize<HashTableLinear>("HashTableLinear", n, csvFile);
        std::cout << "\n";
    }

    std::cout << "Wyniki zapisano do pliku: " << csvFile << "\n";
}

void Menu::run() {
    int option = -1;
    while (option != 0) {
        std::cout << "\n===== MINIPROJEKT 3 - TABLICE MIESZAJACE =====\n"
                  << " 1. Tryb interaktywny (dodawanie / usuwanie / szukanie / wyswietlanie)\n"
                  << " 2. Pomiary wydajnosci (zapis do wyniki.csv)\n"
                  << " 0. Wyjscie\n"
                  << "Wybor: ";

        if (!(std::cin >> option)) break;

        if (option == 1) {
            runInteractive();
        } else if (option == 2) {
            runBenchmarks();
        }
    }

    std::cout << "Koniec programu.\n";
}
