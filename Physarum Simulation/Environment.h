#pragma once
#include <vector>
#include <GLFW/glfw3.h>
#include "GlobalConstants.h"
#include "Agent.h"

class Environment
{
public:
    Environment();

    void display();
    void update();

private:
    std::vector<Agent> agents;
    std::vector<std::pair<float, int>> trailMap;

    void diffusePheromones(float x, float y);
    void decayPheromones(float x, float y);
};