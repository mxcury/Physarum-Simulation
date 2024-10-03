#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H

#include <random>
#include <numbers>
#include <tuple>
#include <SDL.h>
#include <iostream>
#include "SpawnSettings.h"

const int FPS = 60;

const float PI = std::numbers::pi_v<float>;

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

const unsigned int RANDOM_SEED = 010203;

// Colour Settings
const std::tuple<int, int, int> DEFAULT_HIGH_PHEROMONE_COLOUR = { 255, 0, 0 };
const std::tuple<int, int, int> DEFAULT_LOW_PHEROMONE_COLOUR = { 0, 0, 255 };

extern std::tuple<int, int, int> HIGH_PHEROMONE_COLOUR;
extern std::tuple<int, int, int> LOW_PHEROMONE_COLOUR;

const bool RANDOM_COLOUR =  false;

// Spawn Settings
const SpawnPosition SPAWN_POSITION = SpawnPosition::CIRCLE;
const InitialHeading INITIAL_HEADING = InitialHeading::EDGE;
const int SPAWN_RADIUS = 150;

const int SPAWN_COUNT = 10000;

// Agent Settings
const float ROTATION_ANGLE = 0.125f;

const float SENSOR_ANGLE = 0.125f;
const int SENSOR_DISTANCE = 20;

const float RANDOM_MOVEMENT_CHANCE = 0.5f;
const float DIRECTION_BIAS = 0.5f;

const float PHEROMONE_SPREAD = 0.5f;

const int LIFESPAN = 100;
const int REPRODUCTION_VALUE = 500;
const float PHEROMONE_THRESHOLD = 0.005f;

const float REPRODUCE_CHANCE = 0.025f;
const float SURVIVAL_CHANCE = 0.1f;

// Environment Settings
const float DECAY_RATE = 0.05f;
const BoundaryCollision BOUNDARY_COLLISION = BoundaryCollision::CONTAINED;

const int KERNAL_SIZE = 3;

// Helper functions
float random();

float valueToRadians(float value);
float radiansToValue(float radians);

std::tuple<int, int, int> generateRandomColour();
#endif