#include <stdio.h>
#include <SDL.h>
#include <glad/gl.h>

SDL_Window *window;
SDL_GLContext context;

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("Cybercon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext(window);

    int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    printf("OpenGL version %d.%d", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

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

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}