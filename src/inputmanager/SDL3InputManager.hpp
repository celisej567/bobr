#pragma once

#include "inputmanager.hpp"
#include "SDL3/SDL.h"
#include "SDL3/SDL_mouse.h"
#include <cstring>
#include <array>

class CSDL3InputManager : public IInputManager
{
public:
    CSDL3InputManager();
    ~CSDL3InputManager();

    virtual bool Initialize(IWindow* hWindow) override;
    virtual void Shutdown() override;

    virtual void PollEvents() override;

    virtual bool IsKeyDown(KeyCode key) const override;
    virtual bool IsKeyPressed(KeyCode key) const override;
    virtual bool IsKeyReleased(KeyCode key) const override;

    virtual bool IsMouseButtonDown(MouseButton btn) const override;
    virtual bool IsMouseButtonPressed(MouseButton btn) const override;
    virtual bool IsMouseButtonReleased(MouseButton btn) const override;

    virtual float GetMouseX() const override { return m_fMouseX; }
    virtual float GetMouseY() const override { return m_fMouseY; }
    virtual float GetMouseDeltaX() const override { return m_fMouseDeltaX; }
    virtual float GetMouseDeltaY() const override { return m_fMouseDeltaY; }
    virtual float GetScrollDelta() const override { return m_fScrollDelta; }

    virtual bool GetExitFlag() const override { return m_bExitFlag; }

private:
    void ResetKeyStates();
    void UpdateKeyState(KeyCode key, bool bDown);
    void EndFrame();

    static KeyCode SDLScancodeToKeyCode(SDL_Scancode scancode);
    static MouseButton SDLMouseButtonToMouseButton(Uint8 sdlButton);

    IWindow* m_pWindow = nullptr;

    // Keyboard state arrays
    static constexpr int KEY_COUNT = static_cast<int>(KeyCode::Rhyper) + 1;
    bool m_KeyDown[KEY_COUNT];      // currently holding
    bool m_KeyPressed[KEY_COUNT];    // just got pressed down in current frame
    bool m_KeyReleased[KEY_COUNT];   // just got released in current frame

    // Mouse button state
    static constexpr int MOUSE_BUTTON_COUNT = static_cast<int>(MouseButton::Count);
    bool m_MouseDown[MOUSE_BUTTON_COUNT];
    bool m_MousePressed[MOUSE_BUTTON_COUNT];
    bool m_MouseReleased[MOUSE_BUTTON_COUNT];

    // Mouse position / motion
    float m_fMouseX = 0.0f;
    float m_fMouseY = 0.0f;
    float m_fMouseDeltaX = 0.0f;
    float m_fMouseDeltaY = 0.0f;
    float m_fScrollDelta = 0.0f;

    bool m_bExitFlag = false;
};