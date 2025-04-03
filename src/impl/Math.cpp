#include <impl/Math.hpp>
#include <random>
#include <chrono>

Random randomNumberGenerator(
    (static_cast<uint64_t>(std::random_device{}()) << 32) | 
    std::random_device{}() ^ 
    std::chrono::high_resolution_clock::now().time_since_epoch().count()
);

double Math::random() {
    return randomNumberGenerator.nextDouble();
}