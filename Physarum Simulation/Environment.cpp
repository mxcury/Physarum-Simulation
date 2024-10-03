#include "Environment.h"

Environment::Environment() : trailMap(WINDOW_WIDTH * WINDOW_HEIGHT, std::make_pair(0.0f, -1)), numberOfAgents(SPAWN_COUNT)
{
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

void Environment::display(SDL_Renderer* renderer) {
    const auto& [highR, highG, highB] = HIGH_PHEROMONE_COLOUR;
    const auto& [lowR, lowG, lowB] = LOW_PHEROMONE_COLOUR;

    for (int y = 0; y < WINDOW_HEIGHT; ++y) {
        for (int x = 0; x < WINDOW_WIDTH; ++x) {
            int index = y * WINDOW_WIDTH + x;

            float pheromone = trailMap[index].first;

            if (pheromone <= 0.0f) continue;

            float interpR = lowR + (highR - lowR) * pheromone;
            float interpG = lowG + (highG - lowG) * pheromone;
            float interpB = lowB + (highB - lowB) * pheromone;

            Uint8 alpha = static_cast<Uint8>(pheromone * 255);

            SDL_SetRenderDrawColor(renderer,
                static_cast<Uint8>(interpR),
                static_cast<Uint8>(interpG),
                static_cast<Uint8>(interpB),
                alpha);

            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}


void Environment::update() {
    std::vector<Agent> newAgents;
    std::vector<int> agentsToRemove;

    std::cout << agents.size() << std::endl;

    for (int i = 0; i < agents.size(); ++i) {
        if (agents[i].isAlive()) {
            agents[i].update();

            if (agents[i].getReproduce()) {
                float oppositeHeading = fmod(agents[i].getHeading() + 0.5f, 1.0f);
                int newAgentID;
                newAgentID = ++numberOfAgents;
                newAgents.push_back(Agent(newAgentID, agents[i].getX(), agents[i].getY(), oppositeHeading, trailMap));
                agents[i].setReproduce(false);
            }
        }
        else {
            {
                agentsToRemove.push_back(i); 
            }
        }
    }

    std::sort(agentsToRemove.rbegin(), agentsToRemove.rend());
    for (int i : agentsToRemove) {
        agents.erase(agents.begin() + i);
    }

    agents.insert(agents.end(), newAgents.begin(), newAgents.end());

    for (int y = 0; y < WINDOW_HEIGHT; ++y) {
        for (int x = 0; x < WINDOW_WIDTH; ++x) {
            diffuse(x, y);

            if (trailMap.at(y * WINDOW_WIDTH + x).first > PHEROMONE_THRESHOLD) {
                decay(x, y);
            }
        }
    }

}

inline void Environment::diffuse(int x, int y) {
    int index = y * WINDOW_WIDTH + x;

    if (index < 0 || index >= static_cast<int>(trailMap.size())) return;

    float sum = 0.0f;
    int count = 0;

    int radius = KERNAL_SIZE / 2;

    int xStart = std::max(0, x - radius);
    int xEnd = std::min(static_cast<int>(WINDOW_WIDTH - 1), x + radius);
    int yStart = std::max(0, y - radius);
    int yEnd = std::min(static_cast<int>(WINDOW_HEIGHT - 1), y + radius);

    for (int ny = yStart; ny <= yEnd; ++ny) {
        for (int nx = xStart; nx <= xEnd; ++nx) {
            int neighborIndex = ny * WINDOW_WIDTH + nx;
            sum += trailMap[neighborIndex].first;
            count++;
        }
    }

    if (count > 0) {
        float diffusion = sum / count;
        trailMap[index].first = diffusion;
    }
}


void Environment::decay(int x, int y) {
    int index = y * WINDOW_WIDTH + x;

    if (index < 0 || index >= static_cast<int>(trailMap.size())) return;

    trailMap[index].first *= (1.0f - DECAY_RATE);
    trailMap[index].first = std::max(trailMap[index].first, PHEROMONE_THRESHOLD);

    if (trailMap[index].first < PHEROMONE_THRESHOLD) {
        trailMap[index].second = -1;
    }
}
