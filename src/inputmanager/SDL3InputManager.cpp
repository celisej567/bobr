#include "SDL3InputManager.hpp"
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_scancode.h>

//TODO: Remove this
class IWindow
{
public:
    virtual ~IWindow() = default;
    
    virtual void* get() = 0;
    virtual void SetRelativeMouse(bool bEnabled) = 0;
    virtual bool GetRelativeMouse() = 0;
    virtual void ToggleRelativeMouse() = 0;
};

CSDL3InputManager::CSDL3InputManager()
{
    ResetKeyStates();
}

CSDL3InputManager::~CSDL3InputManager()
{
    Shutdown();
}

bool CSDL3InputManager::Initialize(IWindow* hWindow)
{
    m_pWindow = hWindow;
    return true;
}

void CSDL3InputManager::Shutdown()
{
    m_pWindow = nullptr;
}

void CSDL3InputManager::PollEvents()
{
    // Reset per-frame flags only (keep m_KeyDown/m_MouseDown persistent)
    memset(m_KeyPressed, false, KEY_COUNT);
    memset(m_KeyReleased, false, KEY_COUNT);
    memset(m_MousePressed, false, MOUSE_BUTTON_COUNT);
    memset(m_MouseReleased, false, MOUSE_BUTTON_COUNT);
    m_fMouseDeltaX = 0.0f;
    m_fMouseDeltaY = 0.0f;
    m_fScrollDelta = 0.0f;

    if (!m_pWindow)
        return;

    SDL_Window* sdlWindow = static_cast<SDL_Window*>(m_pWindow->get());

    // Sync keyboard state from SDL_GetKeyboardState
    // This gives us reliable "held" state even if events were missed
    const bool* sdlKeys = SDL_GetKeyboardState(NULL);
    for (int i = 0; i < SDL_SCANCODE_COUNT; i++)
    {
        KeyCode key = SDLScancodeToKeyCode(static_cast<SDL_Scancode>(i));
        if (key != KeyCode::Unknown)
        {
            bool bDown = sdlKeys[i];
            int idx = static_cast<int>(key);
            if (bDown && !m_KeyDown[idx])
            {
                m_KeyPressed[idx] = true;
            }
            else if (!bDown && m_KeyDown[idx])
            {
                m_KeyReleased[idx] = true;
            }
            m_KeyDown[idx] = bDown;
        }
    }

    // Process SDL event queue for mouse and window events
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_EVENT_QUIT:
            {
                m_bExitFlag = true;
                break;
            }

            case SDL_EVENT_MOUSE_MOTION:
            {
                m_fMouseX = event.motion.x;
                m_fMouseY = event.motion.y;
                m_fMouseDeltaX += event.motion.xrel;
                m_fMouseDeltaY += event.motion.yrel;
                break;
            }

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                MouseButton btn = SDLMouseButtonToMouseButton(event.button.button);
                if (btn != MouseButton::Count)
                {
                    bool bDown = (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN);
                    int idx = static_cast<int>(btn);
                    if (bDown && !m_MouseDown[idx])
                        m_MousePressed[idx] = true;
                    else if (!bDown && m_MouseDown[idx])
                        m_MouseReleased[idx] = true;
                    m_MouseDown[idx] = bDown;
                }
                break;
            }

            case SDL_EVENT_MOUSE_WHEEL:
            {
                m_fScrollDelta += event.wheel.y;
                break;
            }

            default:
                break;
        }
    }
}

bool CSDL3InputManager::IsKeyDown(KeyCode key) const
{
    int idx = static_cast<int>(key);
    if (idx < 0 || idx >= KEY_COUNT)
        return false;
    return m_KeyDown[idx];
}

bool CSDL3InputManager::IsKeyPressed(KeyCode key) const
{
    int idx = static_cast<int>(key);
    if (idx < 0 || idx >= KEY_COUNT)
        return false;
    return m_KeyPressed[idx];
}

bool CSDL3InputManager::IsKeyReleased(KeyCode key) const
{
    int idx = static_cast<int>(key);
    if (idx < 0 || idx >= KEY_COUNT)
        return false;
    return m_KeyReleased[idx];
}

bool CSDL3InputManager::IsMouseButtonDown(MouseButton btn) const
{
    int idx = static_cast<int>(btn);
    if (idx < 0 || idx >= MOUSE_BUTTON_COUNT)
        return false;
    return m_MouseDown[idx];
}

bool CSDL3InputManager::IsMouseButtonPressed(MouseButton btn) const
{
    int idx = static_cast<int>(btn);
    if (idx < 0 || idx >= MOUSE_BUTTON_COUNT)
        return false;
    return m_MousePressed[idx];
}

bool CSDL3InputManager::IsMouseButtonReleased(MouseButton btn) const
{
    int idx = static_cast<int>(btn);
    if (idx < 0 || idx >= MOUSE_BUTTON_COUNT)
        return false;
    return m_MouseReleased[idx];
}

void CSDL3InputManager::ResetKeyStates()
{
    memset(m_KeyDown, false, KEY_COUNT);
    memset(m_KeyPressed, false, KEY_COUNT);
    memset(m_KeyReleased, false, KEY_COUNT);

    memset(m_MouseDown, false, MOUSE_BUTTON_COUNT);
    memset(m_MousePressed, false, MOUSE_BUTTON_COUNT);
    memset(m_MouseReleased, false, MOUSE_BUTTON_COUNT);
}
void CSDL3InputManager::UpdateKeyState(KeyCode key, bool bDown)
{
    int idx = static_cast<int>(key);
    if (idx < 0 || idx >= KEY_COUNT)
        return;

    if (bDown && !m_KeyDown[idx])
        m_KeyPressed[idx] = true;
    else if (!bDown && m_KeyDown[idx])
        m_KeyReleased[idx] = true;

    m_KeyDown[idx] = bDown;
}

MouseButton CSDL3InputManager::SDLMouseButtonToMouseButton(Uint8 sdlButton)
{
    switch (sdlButton)
    {
        case SDL_BUTTON_LEFT:   return MouseButton::Left;
        case SDL_BUTTON_RIGHT:  return MouseButton::Right;
        case SDL_BUTTON_MIDDLE: return MouseButton::Middle;
        case SDL_BUTTON_X1:     return MouseButton::X1;
        case SDL_BUTTON_X2:     return MouseButton::X2;
        default:                return MouseButton::Count;
    }
}

KeyCode CSDL3InputManager::SDLScancodeToKeyCode(SDL_Scancode scancode)
{
    switch (scancode)
    {
        case SDL_SCANCODE_RETURN:       return KeyCode::Return;
        case SDL_SCANCODE_ESCAPE:       return KeyCode::Escape;
        case SDL_SCANCODE_BACKSPACE:    return KeyCode::Backspace;
        case SDL_SCANCODE_TAB:          return KeyCode::Tab;
        case SDL_SCANCODE_SPACE:        return KeyCode::Space;

        case SDL_SCANCODE_A:            return KeyCode::A;
        case SDL_SCANCODE_B:            return KeyCode::B;
        case SDL_SCANCODE_C:            return KeyCode::C;
        case SDL_SCANCODE_D:            return KeyCode::D;
        case SDL_SCANCODE_E:            return KeyCode::E;
        case SDL_SCANCODE_F:            return KeyCode::F;
        case SDL_SCANCODE_G:            return KeyCode::G;
        case SDL_SCANCODE_H:            return KeyCode::H;
        case SDL_SCANCODE_I:            return KeyCode::I;
        case SDL_SCANCODE_J:            return KeyCode::J;
        case SDL_SCANCODE_K:            return KeyCode::K;
        case SDL_SCANCODE_L:            return KeyCode::L;
        case SDL_SCANCODE_M:            return KeyCode::M;
        case SDL_SCANCODE_N:            return KeyCode::N;
        case SDL_SCANCODE_O:            return KeyCode::O;
        case SDL_SCANCODE_P:            return KeyCode::P;
        case SDL_SCANCODE_Q:            return KeyCode::Q;
        case SDL_SCANCODE_R:            return KeyCode::R;
        case SDL_SCANCODE_S:            return KeyCode::S;
        case SDL_SCANCODE_T:            return KeyCode::T;
        case SDL_SCANCODE_U:            return KeyCode::U;
        case SDL_SCANCODE_V:            return KeyCode::V;
        case SDL_SCANCODE_W:            return KeyCode::W;
        case SDL_SCANCODE_X:            return KeyCode::X;
        case SDL_SCANCODE_Y:            return KeyCode::Y;
        case SDL_SCANCODE_Z:            return KeyCode::Z;

        case SDL_SCANCODE_1:            return KeyCode::Key1;
        case SDL_SCANCODE_2:            return KeyCode::Key2;
        case SDL_SCANCODE_3:            return KeyCode::Key3;
        case SDL_SCANCODE_4:            return KeyCode::Key4;
        case SDL_SCANCODE_5:            return KeyCode::Key5;
        case SDL_SCANCODE_6:            return KeyCode::Key6;
        case SDL_SCANCODE_7:            return KeyCode::Key7;
        case SDL_SCANCODE_8:            return KeyCode::Key8;
        case SDL_SCANCODE_9:            return KeyCode::Key9;
        case SDL_SCANCODE_0:            return KeyCode::Key0;

        case SDL_SCANCODE_LCTRL:        return KeyCode::Lctrl;
        case SDL_SCANCODE_LSHIFT:       return KeyCode::Lshift;
        case SDL_SCANCODE_LALT:         return KeyCode::Lalt;
        case SDL_SCANCODE_LGUI:         return KeyCode::Lgui;
        case SDL_SCANCODE_RCTRL:        return KeyCode::Rctrl;
        case SDL_SCANCODE_RSHIFT:       return KeyCode::Rshift;
        case SDL_SCANCODE_RALT:         return KeyCode::Ralt;
        case SDL_SCANCODE_RGUI:         return KeyCode::Rgui;

        case SDL_SCANCODE_UP:           return KeyCode::Up;
        case SDL_SCANCODE_DOWN:         return KeyCode::Down;
        case SDL_SCANCODE_LEFT:         return KeyCode::Left;
        case SDL_SCANCODE_RIGHT:        return KeyCode::Right;

        case SDL_SCANCODE_F1:           return KeyCode::F1;
        case SDL_SCANCODE_F2:           return KeyCode::F2;
        case SDL_SCANCODE_F3:           return KeyCode::F3;
        case SDL_SCANCODE_F4:           return KeyCode::F4;
        case SDL_SCANCODE_F5:           return KeyCode::F5;
        case SDL_SCANCODE_F6:           return KeyCode::F6;
        case SDL_SCANCODE_F7:           return KeyCode::F7;
        case SDL_SCANCODE_F8:           return KeyCode::F8;
        case SDL_SCANCODE_F9:           return KeyCode::F9;
        case SDL_SCANCODE_F10:          return KeyCode::F10;
        case SDL_SCANCODE_F11:          return KeyCode::F11;
        case SDL_SCANCODE_F12:          return KeyCode::F12;
        case SDL_SCANCODE_F13:          return KeyCode::F13;
        case SDL_SCANCODE_F14:          return KeyCode::F14;
        case SDL_SCANCODE_F15:          return KeyCode::F15;
        case SDL_SCANCODE_F16:          return KeyCode::F16;
        case SDL_SCANCODE_F17:          return KeyCode::F17;
        case SDL_SCANCODE_F18:          return KeyCode::F18;
        case SDL_SCANCODE_F19:          return KeyCode::F19;
        case SDL_SCANCODE_F20:          return KeyCode::F20;
        case SDL_SCANCODE_F21:          return KeyCode::F21;
        case SDL_SCANCODE_F22:          return KeyCode::F22;
        case SDL_SCANCODE_F23:          return KeyCode::F23;
        case SDL_SCANCODE_F24:          return KeyCode::F24;

        case SDL_SCANCODE_INSERT:       return KeyCode::Insert;
        case SDL_SCANCODE_HOME:         return KeyCode::Home;
        case SDL_SCANCODE_PAGEUP:       return KeyCode::Pageup;
        case SDL_SCANCODE_DELETE:       return KeyCode::Delete;
        case SDL_SCANCODE_END:          return KeyCode::End;
        case SDL_SCANCODE_PAGEDOWN:     return KeyCode::Pagedown;

        case SDL_SCANCODE_CAPSLOCK:     return KeyCode::Capslock;
        case SDL_SCANCODE_PRINTSCREEN:  return KeyCode::Printscreen;
        case SDL_SCANCODE_SCROLLLOCK:   return KeyCode::Scrolllock;
        case SDL_SCANCODE_PAUSE:        return KeyCode::Pause;
        case SDL_SCANCODE_NUMLOCKCLEAR: return KeyCode::NumlockClear;
        case SDL_SCANCODE_MENU:         return KeyCode::Menu;

        case SDL_SCANCODE_KP_DIVIDE:    return KeyCode::KeyPad_Divide;
        case SDL_SCANCODE_KP_MULTIPLY:  return KeyCode::KeyPad_Multiply;
        case SDL_SCANCODE_KP_MINUS:     return KeyCode::KeyPad_Minus;
        case SDL_SCANCODE_KP_PLUS:      return KeyCode::KeyPad_Plus;
        case SDL_SCANCODE_KP_ENTER:     return KeyCode::KeyPad_Enter;
        case SDL_SCANCODE_KP_1:         return KeyCode::KeyPad_1;
        case SDL_SCANCODE_KP_2:         return KeyCode::KeyPad_2;
        case SDL_SCANCODE_KP_3:         return KeyCode::KeyPad_3;
        case SDL_SCANCODE_KP_4:         return KeyCode::KeyPad_4;
        case SDL_SCANCODE_KP_5:         return KeyCode::KeyPad_5;
        case SDL_SCANCODE_KP_6:         return KeyCode::KeyPad_6;
        case SDL_SCANCODE_KP_7:         return KeyCode::KeyPad_7;
        case SDL_SCANCODE_KP_8:         return KeyCode::KeyPad_8;
        case SDL_SCANCODE_KP_9:         return KeyCode::KeyPad_9;
        case SDL_SCANCODE_KP_0:         return KeyCode::KeyPad_0;
        case SDL_SCANCODE_KP_PERIOD:    return KeyCode::KeyPad_Period;
        case SDL_SCANCODE_KP_EQUALS:    return KeyCode::KeyPad_Equals;
        case SDL_SCANCODE_KP_COMMA:     return KeyCode::KeyPad_Comma;

        case SDL_SCANCODE_MINUS:        return KeyCode::Minus;
        case SDL_SCANCODE_EQUALS:       return KeyCode::Equals;
        case SDL_SCANCODE_LEFTBRACKET:  return KeyCode::Leftbracket;
        case SDL_SCANCODE_RIGHTBRACKET: return KeyCode::Rightbracket;
        case SDL_SCANCODE_BACKSLASH:    return KeyCode::Backslash;
        case SDL_SCANCODE_SEMICOLON:    return KeyCode::Semicolon;
        case SDL_SCANCODE_APOSTROPHE:   return KeyCode::Apostrophe;
        case SDL_SCANCODE_GRAVE:        return KeyCode::Grave;
        case SDL_SCANCODE_COMMA:        return KeyCode::Comma;
        case SDL_SCANCODE_PERIOD:       return KeyCode::Period;
        case SDL_SCANCODE_SLASH:        return KeyCode::Slash;

        default:                        return KeyCode::Unknown;
    }
}

static CSDL3InputManager g_SDL3InputManager;

extern "C"
{
INTERFACE_EXPOSE_OBJECT(IInputManager, &g_SDL3InputManager);
}