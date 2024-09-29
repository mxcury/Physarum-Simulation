#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H

#include <random>
#include <numbers>
#include <tuple>
#include <SDL.h>
#include "SpawnSettings.h"

const int FPS = 60;

const int SIZE = 4;

const float PI = std::numbers::pi_v<float>;

const int WINDOW_WIDTH = 320 * SIZE;
const int WINDOW_HEIGHT = 180 * SIZE;

const unsigned int RANDOM_SEED = 1;

// Colour Settings
const std::tuple<int, int, int> DEFAULT_HIGH_PHEROMONE_COLOUR = { 255, 0, 255 };
const std::tuple<int, int, int> DEFAULT_LOW_PHEROMONE_COLOUR = { 0, 255, 255 };

extern std::tuple<int, int, int> HIGH_PHEROMONE_COLOUR;
extern std::tuple<int, int, int> LOW_PHEROMONE_COLOUR;

const bool RANDOM_COLOUR =  true;

// Spawn Settings
const SpawnPosition SPAWN_POSITION = SpawnPosition::RANDOM;
const InitialHeading INITIAL_HEADING = InitialHeading::RANDOM;
const int SPAWN_RADIUS = 5 * SIZE;

const int SPAWN_COUNT = 1000 * SIZE;

// Agent Settings
const float ROTATION_ANGLE = 0.125f;

const float SENSOR_ANGLE = 0.25f;
const int SENSOR_DISTANCE = 10;

const float RANDOM_MOVEMENT_CHANCE = 0.1f;
const float DIRECTION_BIAS = 0.5f;

const float PHEROMONE_SPREAD = 1.0f;

const int LIFESPAN = 50;
const float PHEROMONE_THRESHOLD = 0.005f;
const float SURVIVAL_CHANCE = 0.2f;

// Environment Settings
const float DECAY_RATE = 0.03f;
const BoundaryCollision BOUNDARY_COLLISION = BoundaryCollision::CONTAINED;


// Helper functions
float random();

float valueToRadians(float value);
float radiansToValue(float radians);

std::tuple<int, int, int> generateRandomColour();
#endif