#pragma once
#include "GlobalConstants.h"

class Food
{
public:
    Food(int xCoord, int yCoord, int size, bool consumable, std::vector<std::pair<float, int>>& trailMap);

    void update();

private:
    int x, y;
    int radius;
    bool consumable;

    std::vector<std::pair<float, int>>* trailMap;
};
