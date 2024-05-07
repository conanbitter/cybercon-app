#include <stdio.h>
#include <SDL.h>

SDL_Window *window;

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Cybercon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, 0);

    int working = 1;
    while (working) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    working = 0;
                    break;
            }
        }
        SDL_Delay(5);
        }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}