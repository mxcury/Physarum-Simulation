#include "Agent.h"

Agent::Agent(int id, float xCoord, float yCoord, float heading, std::vector<std::pair<float, int>>& trailMap):
    agentID(id), x(xCoord), y(yCoord), heading(heading), trailMap(&trailMap), alive(true), reproduce(false), iteractions_death(0), iteractions_life(0)
{
	vx = cos(valueToRadians(heading));
	vy = sin(valueToRadians(heading));
}

void Agent::update() {
    float headingRadians = valueToRadians(heading);
    float cosHeading = cos(headingRadians);
    float sinHeading = sin(headingRadians);

    vx = cosHeading;
    vy = sinHeading;

    x += vx;
    y += vy;

    float rightSensorAngle = headingRadians + valueToRadians(SENSOR_ANGLE);
    float leftSensorAngle = headingRadians - valueToRadians(SENSOR_ANGLE);

    frontSensorPosition = { x + SENSOR_DISTANCE * cosHeading, y + SENSOR_DISTANCE * sinHeading };
    rightSensorPosition = { x + SENSOR_DISTANCE * cos(rightSensorAngle), y + SENSOR_DISTANCE * sin(rightSensorAngle) };
    leftSensorPosition = { x + SENSOR_DISTANCE * cos(leftSensorAngle), y + SENSOR_DISTANCE * sin(leftSensorAngle) };

    if (BOUNDARY_COLLISION == BoundaryCollision::CONTAINED) {
        if (x <= 10) {
            x = 10;
            heading = 0.5f - random();
            vx = -vx;
        }
        else if (x >= WINDOW_WIDTH - 10) {
            x = WINDOW_WIDTH - 10;
            heading = 0.5f - random();
            vx = -vx;
        }

        if (y <= 10) {
            y = 10;
            heading = -random();
            vy = -vy;
        }
        else if (y >= WINDOW_HEIGHT - 10) {
            y = WINDOW_HEIGHT - 10;
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
    bool foodDetected = false;

    if (trailMap && !trailMap->empty()) {
        auto getPheromoneValue = [&](std::pair<float, float> sensorPosition) -> float {
            float sensorX = sensorPosition.first;
            float sensorY = sensorPosition.second;

            if (sensorX >= 0 && sensorX < WINDOW_WIDTH && sensorY >= 0 && sensorY < WINDOW_HEIGHT) {
                int index = static_cast<int>(sensorY) * WINDOW_WIDTH + static_cast<int>(sensorX);

                if (index >= 0 && index < trailMap->size()) {
                    float pheromoneValue = trailMap->at(index).first;
                    int detectedMoldId = trailMap->at(index).second;
                    if (pheromoneValue > PHEROMONE_THRESHOLD) {
                        if (detectedMoldId == -2) {
                            foodDetected = true;
                            return pheromoneValue;
                        }
                        else if (agentID != detectedMoldId && detectedMoldId != -1) {
                            interaction = true;
                            return pheromoneValue;
                        }
                        else if (detectedMoldId == agentID || detectedMoldId == -1) {
                            return -pheromoneValue;
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

    if (foodDetected) {
        if (frontSensorActivation >= leftSensorActivation && frontSensorActivation >= rightSensorActivation) {
            heading += 0.0f;
        }
        else if (leftSensorActivation > rightSensorActivation) {
            heading -= ROTATION_ANGLE;
        }
        else {
            heading += ROTATION_ANGLE;
        }
    }
    else {
        if (frontSensorActivation > leftSensorActivation && frontSensorActivation > rightSensorActivation) {
            heading += 0.0f;
        }
        else if (frontSensorActivation < leftSensorActivation && frontSensorActivation < rightSensorActivation) {
            heading += (random() < RANDOM_MOVEMENT_CHANCE) ?
                ((random() < DIRECTION_BIAS) ? -ROTATION_ANGLE : ROTATION_ANGLE) :
                0.0f;
        }
        else {
            heading += (leftSensorActivation < rightSensorActivation) ? ROTATION_ANGLE : -ROTATION_ANGLE;
        }
    }


    if (foodDetected) {
        iteractions_life++;
        iteractions_death = 0;
    }
    else if (interaction) {
        iteractions_death = 0;
    }
    else {
        iteractions_death++;
        iteractions_life = 0;
    }

    if (iteractions_life >= REPRODUCTION_VALUE) {
        if (random() < REPRODUCE_CHANCE) {
            reproduce = true;
        }
        iteractions_life = 0;
    }

    if (iteractions_death >= LIFESPAN) {
        if (random() < SURVIVAL_CHANCE) {
            alive = false;
        }
    }

    int intSpread = static_cast<int>(PHEROMONE_SPREAD);

    for (int i = -intSpread; i <= intSpread; ++i) {
        for (int j = -intSpread; j <= intSpread; ++j) {
            float distance = sqrt(static_cast<float>(i * i + j * j));
            if (distance > PHEROMONE_SPREAD) continue;

            int indexX = static_cast<int>(x + i);
            int indexY = static_cast<int>(y + j);

            if (indexX >= 0 && indexX < WINDOW_WIDTH && indexY >= 0 && indexY < WINDOW_HEIGHT) {
                int index = indexY * WINDOW_WIDTH + indexX;
                if (trailMap->at(index).second == -2) {
                    trailMap->at(index) = std::make_pair(1.0f, -2);
                }
                trailMap->at(index) = std::make_pair(1.0f, agentID);
            }
        }
    }
}

