#include "Environment.h"

Environment::Environment() : trailMap(WINDOW_WIDTH * WINDOW_HEIGHT, std::make_pair(0.0f, -1))
{
    if (RANDOM_COLOUR) {
        HIGH_PHEROMONE_COLOUR = std::make_tuple(
            static_cast<int>(random() * 255.0f),
            static_cast<int>(random() * 255.0f),
            static_cast<int>(random() * 255.0f)
        );

        LOW_PHEROMONE_COLOUR = std::make_tuple(
            static_cast<int>(random() * 255.0f),
            static_cast<int>(random() * 255.0f),
            static_cast<int>(random() * 255.0f)
        );
    }
    else {
        HIGH_PHEROMONE_COLOUR = DEFAULT_HIGH_PHEROMONE_COLOUR;
        LOW_PHEROMONE_COLOUR = DEFAULT_LOW_PHEROMONE_COLOUR;
    }

    for (int agentID = 0; agentID < SPAWN_COUNT; agentID++) {
        float x = 0.0f;
        float y = 0.0f;
        float heading = 0.0f;

        switch (SPAWN_POSITION) {
            case SpawnPosition::RANDOM: {
                x = random() * WINDOW_WIDTH;
                y = random() * WINDOW_HEIGHT;
                break;
            }

            case SpawnPosition::RADIUS: {
                float angle = valueToRadians(random());
                x = WINDOW_WIDTH / 2 + SPAWN_RADIUS * std::cos(angle);
                y = WINDOW_HEIGHT / 2 + SPAWN_RADIUS * std::sin(angle);
                break;
            }

            case SpawnPosition::CIRCLE: {
                do {
                    x = (random() * 2 - 1) * SPAWN_RADIUS;
                    y = (random() * 2 - 1) * SPAWN_RADIUS;
                } while (x * x + y * y > SPAWN_RADIUS * SPAWN_RADIUS); 
                x += WINDOW_WIDTH / 2;
                y += WINDOW_HEIGHT / 2;
                break;
            }

            case SpawnPosition::CENTRE: {
                x = WINDOW_WIDTH / 2;
                y = WINDOW_HEIGHT / 2;
                break;
            }
        }

        switch (INITIAL_HEADING) {
            case InitialHeading::RANDOM: {
                heading = random();
                break;
            }
            
            case InitialHeading::CENTRED: {
                float dx = (WINDOW_WIDTH / 2) - x;
                float dy = (WINDOW_HEIGHT / 2) - y;
                heading = radiansToValue(std::atan2(dy, dx));
                break;
            }

            case InitialHeading::EDGE: {
                float dx = x - (WINDOW_WIDTH / 2);
                float dy = y - (WINDOW_HEIGHT / 2);

                if (dx == 0 && dy == 0) {
                    heading = random();
                }
                else {
                    heading = radiansToValue(std::atan2(dy, dx));
                }
                break;
            }
        }
        agents.push_back(Agent(agentID, x, y, heading, trailMap));
    }
}

void Environment::update() {
    for (Agent& agent : agents) {
        if (!agent.isAlive()) continue;
        agent.update();
    }

    for (float y = 0; y < WINDOW_HEIGHT; ++y) {
        for (float x = 0; x < WINDOW_WIDTH; ++x) {
            diffusePheromones(x, y);
            decayPheromones(x, y);
        }
    }
}

void Environment::display() {
    for (float y = 0; y < WINDOW_HEIGHT; ++y) {
        for (float x = 0; x < WINDOW_WIDTH; ++x) {
            int index = static_cast<int>(y) * WINDOW_WIDTH + static_cast<int>(x);

            float pheromone = trailMap[index].first;

            if (pheromone > 0.0f) {
                const auto& [highR, highG, highB] = HIGH_PHEROMONE_COLOUR;
                const auto& [lowR, lowG, lowB] = LOW_PHEROMONE_COLOUR;

                float interpR = lowR + (highR - lowR) * pheromone;
                float interpG = lowG + (highG - lowG) * pheromone;
                float interpB = lowB + (highB - lowB) * pheromone;

                glColor4f(interpR / 255.0f, interpG / 255.0f, interpB / 255.0f, pheromone);

                glBegin(GL_QUADS);
                glVertex2f(static_cast<GLfloat>(x), static_cast<GLfloat>(y));
                glVertex2f(static_cast<GLfloat>(x + 1), static_cast<GLfloat>(y));
                glVertex2f(static_cast<GLfloat>(x + 1), static_cast<GLfloat>(y + 1));
                glVertex2f(static_cast<GLfloat>(x), static_cast<GLfloat>(y + 1));
                glEnd();
            }
        }
    }
}


void Environment::diffusePheromones(float x, float y) {
    if (x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT) {
        return;
    }

    float sum = 0.0f;
    int count = 0;

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int neighborX = static_cast<int>(x) + dx;
            int neighborY = static_cast<int>(y) + dy;

            if (neighborX >= 0 && neighborX < WINDOW_WIDTH && neighborY >= 0 && neighborY < WINDOW_HEIGHT) {
                int neighborIndex = neighborY * WINDOW_WIDTH + neighborX;

                if (neighborIndex >= 0 && neighborIndex < static_cast<int>(trailMap.size())) {
                    sum += trailMap[neighborIndex].first;
                    count++;
                }
            }
        }
    }

    int index = static_cast<int>(y) * WINDOW_WIDTH + static_cast<int>(x);

    if (index >= 0 && index < static_cast<int>(trailMap.size()) && count > 0) {
        trailMap[index].first = sum / count;
    }
}


void Environment::decayPheromones(float x, float y) {
    if (x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT) {
        return;
    }

    int index = static_cast<int>(y) * WINDOW_WIDTH + static_cast<int>(x);

    if (index >= 0 && index < static_cast<int>(trailMap.size())) {
        trailMap[index].first *= (1.0f - DECAY_RATE);
        trailMap[index].first = std::max(trailMap[index].first, 0.0f);

        if (trailMap[index].first == 0.0f) {
            trailMap[index].second = -1;
        }
    }
}

