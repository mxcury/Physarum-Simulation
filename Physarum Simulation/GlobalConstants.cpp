#include "GlobalConstants.h"

std::mt19937 random_generator(RANDOM_SEED);
std::uniform_real_distribution<float> distribution(0.0f, 1.0f);


float random() {
    return distribution(random_generator);
}

std::tuple<int, int, int> generateRandomColour() {
    return std::make_tuple(
        static_cast<int>(random() * 255),
        static_cast<int>(random() * 255),
        static_cast<int>(random() * 255)
    );
}

std::tuple<int, int, int> HIGH_PHEROMONE_COLOUR = RANDOM_COLOUR ? generateRandomColour() : DEFAULT_HIGH_PHEROMONE_COLOUR;
std::tuple<int, int, int> LOW_PHEROMONE_COLOUR = RANDOM_COLOUR ? generateRandomColour() : DEFAULT_LOW_PHEROMONE_COLOUR;

float valueToRadians(float value) {
    return value * 2.0f * PI;
}

float radiansToValue(float radians) {
    float value = radians / (2.0f * PI);
    value = (value < 0) ? value + 1.0f : (value >= 1.0f ? value - 1.0f : value);
    return value;
}