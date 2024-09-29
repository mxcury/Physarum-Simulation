#include "Agent.h"

Agent::Agent(int id, float xCoord, float yCoord, float heading, std::vector<std::pair<float, int>>& trailMap):
    agentID(id), x(xCoord), y(yCoord), heading(heading), trailMap(trailMap),
    speed(1.0f), alive(true), iterationsWithoutInteraction(0)
{
	vx = cos(valueToRadians(heading));
	vy = sin(valueToRadians(heading));
}

float Agent::getPheromoneValue(std::pair<float, float> sensorPosition, bool& interaction) {
    float sensorX = sensorPosition.first;
    float sensorY = sensorPosition.second;

    if (sensorX >= 0 && sensorX < WINDOW_WIDTH && sensorY >= 0 && sensorY < WINDOW_HEIGHT) {
        int index = static_cast<int>(sensorY) * WINDOW_WIDTH + static_cast<int>(sensorX);

        if (index >= 0 && index < trailMap.size()) {
            if (agentID != trailMap[index].second) {
                interaction = true;
                return trailMap[index].first;
            } else {
                return -trailMap[index].first;
            }
        }
    }
    return 0.0f;
}

void Agent::update() {
	vx = cos(valueToRadians(heading));
	vy = sin(valueToRadians(heading));

	x += vx;
	y += vy;

    int intSpread = static_cast<int>(PHEROMONE_SPREAD);

    for (int i = -intSpread; i <= intSpread; ++i) {
        for (int j = -intSpread; j <= intSpread; ++j) {
            float distance = sqrt(static_cast<float>(i * i + j * j));
            if (distance > PHEROMONE_SPREAD) continue;

            int indexX = static_cast<int>(x + i);
            int indexY = static_cast<int>(y + j);

            if (indexX >= 0 && indexX < WINDOW_WIDTH && indexY >= 0 && indexY < WINDOW_HEIGHT) {
                int index = indexY * WINDOW_WIDTH + indexX;
                trailMap[index] = std::make_pair(1.0f, agentID);
            }
        }
    }


	frontSensorPosition = { x + SENSOR_DISTANCE * cos(valueToRadians(heading)), y + SENSOR_DISTANCE * sin(valueToRadians(heading)) };
	rightSensorPosition = { x + SENSOR_DISTANCE * cos(valueToRadians(heading) + valueToRadians(SENSOR_ANGLE)), y + SENSOR_DISTANCE * sin(valueToRadians(heading) + valueToRadians(SENSOR_ANGLE)) };
	leftSensorPosition = { x + SENSOR_DISTANCE * cos(valueToRadians(heading) - valueToRadians(SENSOR_ANGLE)), y + SENSOR_DISTANCE * sin(valueToRadians(heading) - valueToRadians(SENSOR_ANGLE)) };

 
    if (BOUNDARY_COLLISION == BoundaryCollision::CONTAINED) {
        if (x <= 0) {
            x = 0;
            heading = 0.5f - random();
            vx = -vx;
        }
        else if (x >= WINDOW_WIDTH - 1) {
            x = WINDOW_WIDTH - 1;
            heading = 0.5f - random();
            vx = -vx;
        }

        if (y <= 0) {
            y = 0;
            heading = -random();
            vy = -vy;
        }
        else if (y >= WINDOW_HEIGHT - 1) {
            y = WINDOW_HEIGHT - 1;
            heading = -random();
            vy = -vy;
        }
    }
    else if (BOUNDARY_COLLISION == BoundaryCollision::WRAP) {
        if (x < 0) {
            x = WINDOW_WIDTH - 1;
        }
        else if (x >= WINDOW_WIDTH) {
            x = 0;
        }

        if (y < 0) {
            y = WINDOW_HEIGHT - 1;
        }
        else if (y >= WINDOW_HEIGHT) {
            y = 0;
        }
    }


	float frontSensorActivation = 0.0f;
	float rightSensorActivation = 0.0f;
	float leftSensorActivation = 0.0f;

    bool interaction = false;

    if (!trailMap.empty()) {
        auto getPheromoneValue = [&](std::pair<float, float> sensorPosition) -> float {

            float sensorX = sensorPosition.first;
            float sensorY = sensorPosition.second;

            if (sensorX >= 0 && sensorX < WINDOW_WIDTH && sensorY >= 0 && sensorY < WINDOW_HEIGHT) {
                int index = static_cast<int>(sensorY) * WINDOW_WIDTH + static_cast<int>(sensorX);

                if (index >= 0 && index < trailMap.size()) {
                    if (trailMap[index].first > PHEROMONE_THRESHOLD) {
                        if (agentID != trailMap[index].second && trailMap[index].second != -1) {
                            interaction = true;
                            return trailMap[index].first;
                        }
                        else {
                            return -trailMap[index].first;
                        }
                    }
                }
            }
            return 0.0f;
        };

        frontSensorActivation = getPheromoneValue(frontSensorPosition);
        rightSensorActivation = getPheromoneValue(rightSensorPosition);
        leftSensorActivation = getPheromoneValue(leftSensorPosition);
    }



    if (frontSensorActivation > leftSensorActivation && frontSensorActivation > rightSensorActivation) {
        heading += 0.0f;
    }
    else if (frontSensorActivation < leftSensorActivation && frontSensorActivation < rightSensorActivation) {
        heading += (random() < RANDOM_MOVEMENT_CHANCE) ? ((random() < DIRECTION_BIAS) ? -ROTATION_ANGLE : ROTATION_ANGLE) : 0.0f;
    }
    else if (leftSensorActivation < rightSensorActivation) {
        heading += ROTATION_ANGLE;
    }
    else if (leftSensorActivation > rightSensorActivation) {
        heading -= ROTATION_ANGLE;
    }

    if (interaction) {
        iterationsWithoutInteraction = 0;
    }
    else {
        iterationsWithoutInteraction++;
    }

    if (iterationsWithoutInteraction >= LIFESPAN) {
        if (random() < SURVIVAL_CHANCE) {
            alive = false;
        }
    }
}