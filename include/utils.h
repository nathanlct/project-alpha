#pragma once

#include <SFML/Graphics.hpp>

#include <cmath>
#include <random>


bool RAND_SEED_INITIALIZED = false;

// euclidean distance between two points
float dist(sf::Vector2f p1, sf::Vector2f p2) {
    auto delta = p1 - p2;
    return sqrt(delta.x * delta.x + delta.y * delta.y);
}

// random integer in the range [a, b]
int rand_int(int a, int b) {
    if(!RAND_SEED_INITIALIZED) {
        srand(time(NULL));
        RAND_SEED_INITIALIZED = true;
    }

    return rand() % (b - a + 1) + a;
}