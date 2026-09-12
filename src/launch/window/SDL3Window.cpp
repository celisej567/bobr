#include "SDL3Window.hpp"
#include "Window.hpp"
#include <SDL3/SDL_mouse.h>

CSDL3Window::CSDL3Window()
{
    
    wnd = SDL_CreateWindow("launch", WND_WIDTH, WND_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    //SDL_Renderer* ren = SDL_CreateRenderer(wnd, NULL);

	SDL_SetWindowRelativeMouseMode(wnd, true);
}

CSDL3Window::~CSDL3Window()
{
    SDL_DestroyWindow(wnd);
    wnd = NULL;
}

void CSDL3Window::SetRelativeMouse(bool bEnabled)
{
    SDL_SetWindowRelativeMouseMode(wnd, bEnabled);
}

bool CSDL3Window::GetRelativeMouse()
{
    return SDL_GetWindowRelativeMouseMode(wnd);
}

void CSDL3Window::ToggleRelativeMouse()
{
    SDL_SetWindowRelativeMouseMode(wnd, !SDL_GetWindowRelativeMouseMode(wnd));
}