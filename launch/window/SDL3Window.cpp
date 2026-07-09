#include "SDL3Window.hpp"
#include "Window.hpp"
#include <SDL3/SDL_mouse.h>

SDL3Window::SDL3Window()
{

    SDL_Init(SDL_INIT_VIDEO);
    
    wnd = SDL_CreateWindow("launch", WND_WIDTH, WND_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    //SDL_Renderer* ren = SDL_CreateRenderer(wnd, NULL);

	SDL_SetWindowRelativeMouseMode(wnd, true);
}

SDL3Window::~SDL3Window()
{
    SDL_DestroyWindow(wnd);
    wnd = NULL;
}

void SDL3Window::SetRelativeMouse(bool bEnabled)
{
    SDL_SetWindowRelativeMouseMode(wnd, bEnabled);
}

bool SDL3Window::GetRelativeMouse()
{
    return SDL_GetWindowRelativeMouseMode(wnd);
}

void SDL3Window::ToggleRelativeMouse()
{
    SDL_SetWindowRelativeMouseMode(wnd, !SDL_GetWindowRelativeMouseMode(wnd));
}