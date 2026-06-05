#include "IHashTable.hpp"
#include "HashTableList.hpp"
#include "HashTableAVL.hpp"
#include "HashTableLinear.hpp"
#include "Utils.hpp"
#include "FileManager.hpp"

#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstdio> // std::remove

// Liczba różnych seedów - po nich uśredniamy każdy wynik (powtarzalność badań)
const int SEED_COUNT = 5;

using Clock = std::chrono::high_resolution_clock;

// Zwraca różnicę dwóch chwil czasu w nanosekundach
static long long durationNs(Clock::time_point start, Clock::time_point end) {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

// Generuje n par (klucz, wartość) dla zadanego seeda.
// Ten sam seedIndex zawsze daje ten sam zestaw danych -> badania są powtarzalne.
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
// Dla każdego seeda budujemy świeżą strukturę i mierzymy łączny czas N operacji,
// a następnie uśredniamy wynik i dzielimy przez N (czas pojedynczej operacji).
template <typename TableType>
static void benchmarkSize(const std::string& name, int n, const std::string& csvFile) {
    long long sumInsert = 0, sumFind = 0, sumRemove = 0;

    for (int seed = 0; seed < SEED_COUNT; seed++) {
        std::vector<DataPair> data = generateData(n, seed);
        TableType table;

        // --- INSERT: wstawienie wszystkich N par ---
        Clock::time_point i0 = Clock::now();
        for (const DataPair& p : data) {
            table.insert(p.key, p.value);
        }
        Clock::time_point i1 = Clock::now();

        // --- FIND: wyszukanie wszystkich N kluczy (wszystkie istnieją) ---
        int out_value;
        bool guard = false; // sztuczne użycie wyniku, by kompilator nie usunął find
        Clock::time_point f0 = Clock::now();
        for (const DataPair& p : data) {
            guard ^= table.find(p.key, out_value);
        }
        Clock::time_point f1 = Clock::now();

        // --- REMOVE: usunięcie wszystkich N kluczy ---
        Clock::time_point r0 = Clock::now();
        for (const DataPair& p : data) {
            table.remove(p.key);
        }
        Clock::time_point r1 = Clock::now();

        if (guard) {
            std::cout << ""; // nigdy nic nie wypisze, ale "zużywa" guard
        }

        sumInsert += durationNs(i0, i1);
        sumFind   += durationNs(f0, f1);
        sumRemove += durationNs(r0, r1);
    }

    // Średni czas POJEDYNCZEJ operacji: dzielimy przez liczbę seedów i przez N
    long long divisor = (long long)SEED_COUNT * n;
    long long avgInsert = sumInsert / divisor;
    long long avgFind   = sumFind / divisor;
    long long avgRemove = sumRemove / divisor;

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

// Krótki test poprawności pojedynczej struktury (przez wspólny interfejs)
static void demoStructure(const std::string& name, IHashTable& table) {
    std::cout << "\n[" << name << "] test poprawnosci:\n";
    table.insert(10, 100);
    table.insert(20, 200);
    table.insert(30, 300);
    table.insert(20, 999); // aktualizacja istniejącego klucza

    int value;
    if (table.find(20, value)) {
        std::cout << "  find(20) = " << value << " (oczekiwane: 999)\n";
    }

    table.remove(10);
    if (!table.find(10, value)) {
        std::cout << "  remove(10) OK - klucza juz nie ma\n";
    }

    table.display();
}

// Sprawdza, czy wszystkie trzy struktury dzialaja tak samo
static void runDemo() {
    std::cout << "===== DEMONSTRACJA POPRAWNOSCI STRUKTUR =====\n";
    HashTableList list;
    demoStructure("HashTableList", list);

    HashTableAVL avl;
    demoStructure("HashTableAVL", avl);

    HashTableLinear linear;
    demoStructure("HashTableLinear", linear);
}

int main() {
    const std::string csvFile = "wyniki.csv";

    // Usuwamy stary plik, bo saveToCSV tylko dopisuje wiersze
    std::remove(csvFile.c_str());

    // 1. Demonstracja, że struktury działają poprawnie
    runDemo();

    // 2. Pomiary wydajności
    // Minimum 8 punktów pomiarowych, start od N=5000 (zgodnie z wytycznymi)
    const int sizes[] = {5000, 10000, 20000, 40000, 80000, 160000, 320000, 640000};
    const int sizeCount = sizeof(sizes) / sizeof(sizes[0]);

    std::cout << "\n\n===== POMIARY WYDAJNOSCI =====\n";
    std::cout << "Sredni czas pojedynczej operacji w nanosekundach [ns], "
              << "usredniony po " << SEED_COUNT << " seedach.\n\n";

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
    return 0;
}
