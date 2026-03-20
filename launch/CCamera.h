#pragma once
#include "glad/glad.h"
#include "glm.hpp"
#include "SDL3/SDL.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "ICamera.h"
#include "mytypes.h"

class CCamera : public ICamera
{
public:
    CCamera();
    CCamera(glm::vec3 position, glm::vec3 angles, float flFloat);
    //CCamera(glm::vec3 position, glm::vec3 angle, glm::vec3 forward, glm::vec3 right, glm::vec3 up);

    virtual glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
    }

    virtual float GetFov() { return m_flFov; }
    virtual void SetFov(float flFov) { 
        m_flFov = flFov;
        
        if(m_flFov > 179)
			m_flFov = 179;

		if(m_flFov < 1)
			m_flFov = 1;
    }

    virtual void ProcessSDLKeyInput(const bool* key_states, float deltaTime);
    virtual void ProcessSDLMouseInput(SDL_Event& event, float deltaTime);

protected:

    void UpdateCameraWorldAngles();

    // use only if UpdateCameraWorldAngles has already been called
    //void UpdateCameraLocalAngles();

    glm::vec3 m_Position;
    glm::vec3 m_Angles;
    glm::vec3 m_Forward;
    glm::vec3 m_Right;
    glm::vec3 m_Up;
    float m_flFov;

};