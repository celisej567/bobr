#pragma once

#include "Window.hpp"
#include "SDL3/SDL.h"

class SDL3Window : public IWindow
{
public:
    SDL3Window();
    ~SDL3Window();
    virtual void* get() {return wnd;};
    virtual void SetRelativeMouse(bool bEnabled);
    virtual bool GetRelativeMouse();
    virtual void ToggleRelativeMouse();
    
private:
    SDL_Window* wnd;
};