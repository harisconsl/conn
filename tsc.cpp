#include <iostream>
#include <chrono>

uint64_t getTscValue() {
    unsigned int low, high;
    asm volatile("rdtsc" : "=a" (low), "=d" (high));
    return ((uint64_t)high << 32) | low;
}

int main() {
    uint64_t start = getTscValue();

    // Code to measure the elapsed time

    uint64_t end = getTscValue();

    uint64_t elapsed = end - start;
    std::cout << "Elapsed TSC cycles: " << elapsed << std::endl;

    return 0;
}
