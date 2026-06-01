#include "FileManager.hpp"
#include "Utils.hpp" 
#include <fstream>
#include <iostream>
#include <stdexcept>

void FileManager::generateInputFile(const std::string& filename, int n) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Nie mozna utworzyc pliku: " + filename);
    }

    RandomGenerator rng;
    file << n << "\n";
    for (int i = 0; i < n; ++i) {
        file << rng.generateKey(n) << " " << rng.generateValue() << "\n";
    }
    
    file.close();
    std::cout << "Wygenerowano plik: " << filename << " z " << n << " elementami." << std::endl;
}

std::vector<DataPair> FileManager::loadFromFile(const std::string& filename) {
    std::vector<DataPair> pairs;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Nie mozna otworzyc pliku: " + filename);
    }

    int n;
    if (!(file >> n)) return pairs;

    pairs.reserve(n);
    int k, v;
    for (int i = 0; i < n; ++i) {
        if (file >> k >> v) {
            pairs.push_back({k, v});
        }
    }
    
    file.close();
    return pairs;
}

void FileManager::saveToCSV(const std::string& filename, const std::string& structureName, 
                            const std::string& operationName, int n, long long timeNs) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Blad: Nie udalo sie otworzyc pliku CSV do zapisu." << std::endl;
        return;
    }

    file.seekp(0, std::ios::end);
    if (file.tellp() == 0) {
        file << "Structure,Operation,N,Time_ns\n";
    }

    file << structureName << "," << operationName << "," << n << "," << timeNs << "\n";
    file.close();
}