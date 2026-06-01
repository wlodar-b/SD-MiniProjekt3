#include "Utils.hpp"

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