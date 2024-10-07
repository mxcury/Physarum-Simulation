#include <iostream>
#include <fstream>      // For file handling
#include <SDL.h>
#include <chrono>
#include "Environment.h"
#include "GlobalConstants.h"

const int frameDelay = 1000 / FPS;

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create an SDL window
    SDL_Window* window = SDL_CreateWindow("Physarum Simulation",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Create an SDL renderer with hardware acceleration
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Enable alpha blending mode
    if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) != 0) {
        std::cerr << "Failed to enable alpha blending: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    Environment env;

    // Open CSV file for writing
    std::ofstream csvFile("agent_data.csv");
    if (!csvFile.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return -1;
    }

    // Write the header to the CSV file
    csvFile << "Iteration,AgentCount\n";

    bool running = true;
    SDL_Event event;

    int frameCount = 0;
    auto startTime = std::chrono::high_resolution_clock::now();

    // Main loop
    int extraIterations = 0; // Counter for additional iterations after emptyTrail returns true

    while (running) {
        auto frameStart = std::chrono::high_resolution_clock::now();

        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        env.update();

        // If the trail is empty and the extra iterations counter hasn't been set
        if (env.emptyTrail() && extraIterations == 0) {
            extraIterations = 50;  // Start counting down the extra iterations
        }

        // If we are in the extra iteration phase, decrement the counter
        if (extraIterations > 0) {
            extraIterations--;
            if (extraIterations == 0) {
                running = false;  // Stop after 50 extra iterations
            }
        }

        // Log the current iteration and agent count to the CSV file
        int agentCount = env.getAgentCount() - SPAWN_COUNT;
        csvFile << frameCount << "," << agentCount << "\n";

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        env.display(renderer);

        SDL_RenderPresent(renderer);

        auto frameEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float, std::milli> frameDuration = frameEnd - frameStart;

        int frameTime = static_cast<int>(frameDuration.count());
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }

        frameCount++;  // Increment the iteration count
    }


    // Close the CSV file
    csvFile.close();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
