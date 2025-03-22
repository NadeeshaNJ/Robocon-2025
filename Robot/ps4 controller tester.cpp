#include <SDL2/SDL.h>
#include <iostream>

int main() {
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Check if a controller is connected
    if (SDL_NumJoysticks() < 1) {
        std::cerr << "No controllers connected!" << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_GameController* controller = SDL_GameControllerOpen(0);
    if (!controller) {
        std::cerr << "Failed to open controller: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    std::cout << "PS4 Controller connected! Press L3 or R3 to test." << std::endl;

    SDL_Event event;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_CONTROLLERBUTTONDOWN) {
                if (event.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSTICK) {
                    std::cout << "L3 Button Pressed!" << std::endl;
                }
                if (event.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSTICK) {
                    std::cout << "R3 Button Pressed!" << std::endl;
                }
            }
        }
    }

    SDL_GameControllerClose(controller);
    SDL_Quit();
    return 0;
}
