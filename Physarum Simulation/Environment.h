#pragma once
#include <vector>
#include "GlobalConstants.h"
#include "Food.h"
#include "Agent.h"

class Environment
{
public:
    Environment();

    void display(SDL_Renderer* renderer);
    void update();
    int getAgentCount() {
        return static_cast<int>(agents.size());
    }
    bool emptyTrail() {
        for (const auto& entry : trailMap) {
            if (entry.first > PHEROMONE_THRESHOLD) {
                return false;
            }
        }
        return true;
    }

private:
    std::vector<Agent> agents;
    std::vector<Food> food;
    std::vector<std::pair<float, int>> trailMap;

    void diffuse(int x, int y);
    void decay(int x, int y);

    int numberOfAgents;

};