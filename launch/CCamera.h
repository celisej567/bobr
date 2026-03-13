#pragma once
#include "glad/glad.h"
#include "glm.hpp"
#include "SDL3/SDL.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

const glm::vec3 g_WorldUp = glm::vec3(0,1,0);

const float flCameraMoveSpeed = 5;
const float flCameraMoveSpeedFast = 10;
const float flCameraSensivity = 0.1;

class CCamera
{
public:
    CCamera();
    CCamera(glm::vec3 position, glm::vec3 angles, float flFloat);
    //CCamera(glm::vec3 position, glm::vec3 angle, glm::vec3 forward, glm::vec3 right, glm::vec3 up);

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
    }

    float GetFov() { return m_flFov; }
    void SetFov(float flFov) { 
        m_flFov = flFov;
        
        if(m_flFov > 179)
			m_flFov = 179;

		if(m_flFov < 1)
			m_flFov = 1;
    }

    void ProcessSDLKeyInput(const bool* key_states, float deltaTime);
    void ProcessSDLMouseInput(SDL_Event& event, float deltaTime);

private:

    void UpdateCameraWorldAngles();

    glm::vec3 m_Position;
    glm::vec3 m_Angles;
    glm::vec3 m_Forward;
    glm::vec3 m_Right;
    glm::vec3 m_Up;
    float m_flFov;

};