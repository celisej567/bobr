#pragma once
#include "SDL3/SDL.h"
#include "ICamera.h"
#include "glm.hpp"
#include "CEntity.h"
#include "mytypes.h"

const glm::vec3 g_WorldUp = glm::vec3(0,1,0);

const float flCameraMoveSpeed = 5;
const float flCameraMoveSpeedFast = 10;
const float flCameraSensivity = 0.1;

inline float curtime = 0;

inline ICamera* g_pActiveCamera;

inline SDL_Window* wnd;

inline glm::mat4 projection;
inline glm::mat4 view = glm::mat4(1);

#define BASECLASS(thisclass,baseclass)  typedef baseclass BaseClass; typedef thisclass ThisClass;