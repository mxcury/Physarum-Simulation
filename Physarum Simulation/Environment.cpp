#include <omp.h>
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
#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < agents.size(); ++i) {
        if (agents[i].isAlive()) agents[i].update();
    }


    #pragma omp parallel for collapse(2) schedule(static)
    for (float y = 0; y < WINDOW_HEIGHT; ++y) {
        for (float x = 0; x < WINDOW_WIDTH; ++x) {
            diffusePheromones(x, y);
        }
    }

    #pragma omp parallel for collapse(2) schedule(static)
    for (float y = 0; y < WINDOW_HEIGHT; ++y) {
        for (float x = 0; x < WINDOW_WIDTH; ++x) {
            decayPheromones(x, y);
        }
    }
}




void Environment::display(SDL_Renderer* renderer) {
    const auto& [highR, highG, highB] = HIGH_PHEROMONE_COLOUR;
    const auto& [lowR, lowG, lowB] = LOW_PHEROMONE_COLOUR;

    for (int y = 0; y < WINDOW_HEIGHT; ++y) {
        for (int x = 0; x < WINDOW_WIDTH; ++x) {
            int index = y * WINDOW_WIDTH + x;

            float pheromone = trailMap[index].first;

            if (pheromone > 0.0f) {
                float interpR = lowR + (highR - lowR) * pheromone;
                float interpG = lowG + (highG - lowG) * pheromone;
                float interpB = lowB + (highB - lowB) * pheromone;

                SDL_SetRenderDrawColor(renderer,
                    static_cast<Uint8>(interpR),
                    static_cast<Uint8>(interpG),
                    static_cast<Uint8>(interpB),
                    static_cast<Uint8>(pheromone * 255.0f));

                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}


void Environment::diffusePheromones(float x, float y) {
    int index = static_cast<int>(y) * WINDOW_WIDTH + static_cast<int>(x);

    if (index < 0 || index >= trailMap.size()) return;

    float sum = 0.0f;
    int count = 0;

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int neighborX = static_cast<int>(x) + dx;
            int neighborY = static_cast<int>(y) + dy;

            if (neighborX >= 0 && neighborX < WINDOW_WIDTH && neighborY >= 0 && neighborY < WINDOW_HEIGHT) {
                int neighborIndex = neighborY * WINDOW_WIDTH + neighborX;
                sum += trailMap[neighborIndex].first;
                count++;
            }
        }
    }

    if (count > 0) {
        trailMap[index].first = sum / count;
    }
}

void Environment::decayPheromones(float x, float y) {
    int index = static_cast<int>(y) * WINDOW_WIDTH + static_cast<int>(x);

    if (index < 0 || index >= trailMap.size()) return;

    trailMap[index].first *= (1.0f - DECAY_RATE);
    trailMap[index].first = std::max(trailMap[index].first, 0.0f);

    if (trailMap[index].first < PHEROMONE_THRESHOLD) {
        trailMap[index].second = -1;
    }
}




