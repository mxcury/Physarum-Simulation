#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H

#include <random>
#include <numbers>
#include <tuple>
#include "SpawnSettings.h"


const float PI = std::numbers::pi_v<float>;

const int WINDOW_WIDTH = 320 * 4;
const int WINDOW_HEIGHT = 180 * 4;

const std::tuple<int, int, int> DEFAULT_HIGH_PHEROMONE_COLOUR = { 255, 0, 0 };
const std::tuple<int, int, int> DEFAULT_LOW_PHEROMONE_COLOUR = { 0, 0, 255 };

extern std::tuple<int, int, int> HIGH_PHEROMONE_COLOUR;
extern std::tuple<int, int, int> LOW_PHEROMONE_COLOUR;

const bool RANDOM_COLOUR = false;

const SpawnPosition SPAWN_POSITION = SpawnPosition::RANDOM;
const InitialHeading INITIAL_HEADING = InitialHeading::CENTRED;
const int SPAWN_COUNT = 5000;
const int SPAWN_RADIUS = 5;

const float ROTATION_ANGLE = 0.025f;

const float SENSOR_ANGLE = 0.125f;
const int SENSOR_DISTANCE = 30;

const int PHEROMONE_SPREAD = 1;

const float DECAY_RATE = 0.03f;

const int LIFESPAN = 3;
const float PHEROMONE_THRESHOLD = 0.005f;

const unsigned int RANDOM_SEED = 78787;

float random();

float valueToRadians(float value);
float radiansToValue(float radians);
#endif