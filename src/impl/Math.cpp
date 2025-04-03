#include <impl/Math.hpp>

double Math::random() {
    static Random rng;
    
    return rng.nextDouble();
}