#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <string>
#include <vector>

// Struktura pomocnicza używana wyłącznie do operacji plikowych
struct DataPair {
    int key;
    int value;
};

class FileManager {
public:
    // Tworzy plik .txt z losowymi danymi
    static void generateInputFile(const std::string& filename, int n);

    // Wczytuje dane z pliku
    static std::vector<DataPair> loadFromFile(const std::string& filename);

    // Zapisuje wyniki pomiarów do CSV
    static void saveToCSV(const std::string& filename, const std::string& structureName, 
                          const std::string& operationName, int n, long long timeNs);
};

#endif // FILEMANAGER_HPP