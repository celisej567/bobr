#pragma once 

#include "glad/glad.h"
#include "glm.hpp"
#include "SDL3/SDL.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "mytypes.h"

class ICamera
{
public:
    virtual glm::mat4 GetViewMatrix() = 0;
    virtual float GetFov() = 0;
    virtual void SetFov(float flFov) = 0;

    virtual void ProcessSDLKeyInput(const bool* key_states, float deltaTime) = 0;
    virtual void ProcessSDLMouseInput(SDL_Event& event, float deltaTime) = 0;
};
