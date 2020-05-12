// This is a very simple library that displays a window when the `draw' function is called.
// This of course requires the SDL2.

#define SDL_MAIN_HANDLED
#include <SDL2\SDL.h>
#include <stdbool.h>

void draw(const char *title, const int l, const int h)
{
    bool quit = false;
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window =
        SDL_CreateWindow(title,
                         SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, l, h, 0);
    while (!quit)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
}
