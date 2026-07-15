#pragma once

#include "Window.hpp"
#include "SDL3/SDL.h"

class CSDL3Window : public IWindow
{
public:
    CSDL3Window();
    ~CSDL3Window();
    virtual void* get() {return wnd;};
    virtual void SetRelativeMouse(bool bEnabled);
    virtual bool GetRelativeMouse();
    virtual void ToggleRelativeMouse();
    
private:
    SDL_Window* wnd;
};