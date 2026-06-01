#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdint> 
#include <limits> 
#include <random> 

class RandomGenerator {
    private:
        static constexpr std::uint32_t SEEDS[5] = {
            123456789u,
            987654321u,
            555666777u,
            111222333u,
            444333222u
        };
        std::mt19937 rng; 
        std::uniform_int_distribution<std::int32_t> valueDist; 

    public:
        RandomGenerator();

        void reset(int seedIndex = 0); 

        // Generowanie losowej wartości
        std::int32_t generateValue();

        // Generowanie losowego klucza (dawniej priorytetu)
        int generateKey(int n); 
};

#endif // UTILS_HPP