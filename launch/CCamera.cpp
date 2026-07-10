#include "CCamera.h"
#include "glm.hpp"

#include "shared.h"

CCamera::CCamera()
{
    m_Position = glm::vec3( 0, 0, 0 );
    m_Angles = glm::vec3( 0, -90, 0 );
    m_flFov = 45;

    UpdateCameraWorldAngles();
}

CCamera::CCamera( glm::vec3 position, glm::vec3 angle, float flFov )
{
    m_Position = position;
    m_Angles = angle;
    m_flFov = flFov;

    UpdateCameraWorldAngles();
}

void CCamera::UpdateCameraWorldAngles()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_Angles[1])) * cos(glm::radians(m_Angles[1]));
    front.y = sin(glm::radians(m_Angles[0]));

    front.z = sin(glm::radians(m_Angles[1])) * cos(glm::radians(m_Angles[0]));
    m_Forward = glm::normalize(front);
    
    m_Right = glm::normalize(glm::cross(m_Forward, g_WorldUp));
    m_Up    = glm::normalize(glm::cross(m_Right, m_Forward));
}