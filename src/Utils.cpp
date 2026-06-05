#include "Utils.hpp"

// Definicja statycznej tablicy poza klasą.
// Wymagana w C++11/14, bo SEEDS[i] pobiera adres elementu (odr-use).
// W C++17 składowe constexpr są domyślnie inline, więc ta linia jest tam nieszkodliwa.
constexpr std::uint32_t RandomGenerator::SEEDS[5];

RandomGenerator::RandomGenerator()
    : rng(SEEDS[0]),
      valueDist(std::numeric_limits<std::int32_t>::min(),
                std::numeric_limits<std::int32_t>::max())
{}

void RandomGenerator::reset(int seedIndex) {
    if (seedIndex >= 0 && seedIndex < 5) {
        rng.seed(SEEDS[seedIndex]); 
    } else {
        rng.seed(SEEDS[0]); 
    }
}

std::int32_t RandomGenerator::generateValue() {
    return valueDist(rng); 
}

int RandomGenerator::generateKey(int n) {
    // Zakres dla klucza (celowo szerszy, aby testować kolizje)
    std::uniform_int_distribution<int> keyDist(0, 10 * n); 
    return keyDist(rng); 
}