#pragma once
#include <vector>
#include "GlobalConstants.h"
#include "Agent.h"

class Environment
{
public:
    Environment();

    void display(SDL_Renderer* renderer);
    void update();

private:
    std::vector<Agent> agents;
    std::vector<std::pair<float, int>> trailMap;

    void diffuse(int x, int y);
    void decay(int x, int y);

    int numberOfAgents;

};