#include "CCamera.h"
#include "glm.hpp"

#include "shared.h"

CCamera::CCamera()
{
    m_Position = glm::vec3( 0, 0, 0 );
    m_Angles = glm::vec3( 0, -90, 0 );
    m_flFov = 45;

    UpdateVectors();
}

CCamera::CCamera( glm::vec3 position, glm::vec3 angle, float flFov )
{
    m_Position = position;
    m_Angles = angle;
    m_flFov = flFov;

    UpdateVectors();
}

void CCamera::UpdateVectors()
{
    float pitch = glm::radians(m_Angles[0]);
    float yaw   = glm::radians(m_Angles[1]); 
    float roll  = glm::radians(m_Angles[2]);

    // Forward from pitch/yaw
    glm::vec3 forward;
    forward.x = cos(yaw) * cos(pitch);
    forward.y = sin(pitch);
    forward.z = sin(yaw) * cos(pitch);
    m_Forward = glm::normalize(forward);

    // right/up
    m_Right = glm::normalize(glm::cross(m_Forward, g_WorldUp));
    m_Up    = glm::normalize(glm::cross(m_Right, m_Forward));

    // roll right/up around forward
    glm::mat4 rollMat = glm::rotate(glm::mat4(1.0f), roll, m_Forward);
    m_Right = glm::mat3(rollMat) * m_Right;
    m_Up    = glm::mat3(rollMat) * m_Up;
}
