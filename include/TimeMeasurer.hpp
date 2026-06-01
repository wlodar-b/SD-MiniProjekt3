#ifndef TIMEMEASURER_HPP
#define TIMEMEASURER_HPP

#include <chrono> 
#include <functional> 

template <typename T>
class TimeMeasurer {
    private:
        T& original; 
        int count; 

    public:
        TimeMeasurer(T& original, int count) : original(original), count(count) {}

        long long measure(std::function<void(T&)> operation) {
            long long totalDuration = 0;
            
            for (int i = 0; i < count; ++i) {
                T copy = original; // Kopia dla pojedynczego pomiaru

                auto start = std::chrono::high_resolution_clock::now();
                operation(copy);
                auto end = std::chrono::high_resolution_clock::now();

                totalDuration += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count(); 
            }

            return totalDuration;
        }
};

#endif // TIMEMEASURER_HPP