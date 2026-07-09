#pragma once

#define WND_WIDTH 800
#define WND_HEIGHT 600

class IWindow
{
public:
    virtual ~IWindow() = default;
    
    virtual void* get() = 0;
    virtual void SetRelativeMouse(bool bEnabled) = 0;
    virtual bool GetRelativeMouse() = 0;
    virtual void ToggleRelativeMouse() = 0;
};