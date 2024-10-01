#pragma once
#include "GlobalConstants.h"

class Agent {
public:
    Agent(int agentID, float xCoord, float yCoord, float heading, std::vector<std::pair<float, int>>& trailMap);

    void update();
    int getID() const { return agentID; }
    float getX() const { return x; }
    float getY() const { return y; }
    float getHeading() const { return heading; }
    bool isAlive() const { return alive; }
    bool getReproduce() const { return reproduce; }

    void setReproduce(bool newValue) { reproduce = newValue; }

private:
    int agentID;
    float x, y;
    float heading, vx, vy, speed;
    bool alive, reproduce;
    int iteractions_death, iteractions_life;

    std::pair<float, float> frontSensorPosition = { 0.0f, 0.0f };
    std::pair<float, float> leftSensorPosition = { 0.0f, 0.0f };
    std::pair<float, float> rightSensorPosition = { 0.0f, 0.0f };

    std::vector<std::pair<float, int>>* trailMap;
};
