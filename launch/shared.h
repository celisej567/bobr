#pragma once

#include "SDL3/SDL.h"
#include "ICamera.h"
#include "glm.hpp"

const glm::vec3 g_WorldUp = glm::vec3(0,1,0);

const float flCameraMoveSpeed = 5;
const float flCameraMoveSpeedFast = 10;
const float flCameraSensivity = 0.1;

inline ICamera* g_pActiveCamera;

inline SDL_Window* wnd;