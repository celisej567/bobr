#pragma once
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "ICamera.h"
#include "mytypes.h"

class CCamera : public ICamera
{
public:
    CCamera();
    CCamera(glm::vec3 position, glm::vec3 angles, float flFloat);

    virtual glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
    }

    virtual float GetFov() { return m_flFov; }
    virtual void SetFov(float flFov) { 
        m_flFov = flFov;
        
        if(m_flFov > 179)
			m_flFov = 179;

		if(m_flFov < 0.001f)
			m_flFov = 0.001f;
    }

    virtual glm::vec3 GetPosition() const { return m_Position; }
    virtual void SetPosition(const glm::vec3& pos) { m_Position = pos; }

    virtual glm::vec3 GetAngles() const { return m_Angles; }
    virtual void SetAngles(const glm::vec3& angles)
    {
        m_Angles = angles;

        UpdateVectors();

    }

    virtual glm::vec3 GetForward() const { return m_Forward; }
    virtual glm::vec3 GetRight() const { return m_Right; }
    virtual glm::vec3 GetUp() const { return m_Up; }

protected:

    void UpdateVectors();

    glm::vec3 m_Position;
    glm::vec3 m_Angles;
    glm::vec3 m_Forward;
    glm::vec3 m_Right;
    glm::vec3 m_Up;
    float m_flFov;

};