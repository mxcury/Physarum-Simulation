#include "Food.h"

Food::Food(int xCoord, int yCoord, int size, bool consumable, std::vector<std::pair<float, int>>& trailMap) :
	x(xCoord), y(yCoord), radius(size), consumable(consumable), trailMap(&trailMap)
{
	
}

void Food::update()
{
    for (int i = -radius; i <= radius; ++i) {
        for (int j = -radius; j <= radius; ++j) {
            float distance = sqrt(static_cast<float>(i * i + j * j));
            if (distance > radius) continue;

            int indexX = static_cast<int>(x + i);
            int indexY = static_cast<int>(y + j);

            if (indexX >= 0 && indexX < WINDOW_WIDTH && indexY >= 0 && indexY < WINDOW_HEIGHT) {
                int index = indexY * WINDOW_WIDTH + indexX;
                trailMap->at(index) = std::make_pair(1.0f, -2);
            }
        }
    }
}


