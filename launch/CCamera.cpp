#include "CCamera.h"
#include "glm.hpp"
#include "SDL3/SDL.h"

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

/*
CCamera::CCamera( glm::vec3 position, glm::vec3 angle, glm::vec3 forward, glm::vec3 right, glm::vec3 up )
{
    m_Position = position;
    m_Angles = angle;

    m_Forward = forward;
    m_Right = right;
    m_Up = up;
}
*/

void CCamera::UpdateCameraWorldAngles()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_Angles[1])) * cos(glm::radians(m_Angles[1]));
    front.y = sin(glm::radians(m_Angles[0]));

    //front.z = sin(glm::radians(m_Angles[2]));
    front.z = sin(glm::radians(m_Angles[1])) * cos(glm::radians(m_Angles[0]));
    m_Forward = glm::normalize(front);
    
    m_Right = glm::normalize(glm::cross(m_Forward, g_WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_Up    = glm::normalize(glm::cross(m_Right, m_Forward));
}

void CCamera::ProcessSDLKeyInput( const bool* key_states, float deltaTime )
{
    float flMoveSpeed = key_states[SDL_SCANCODE_LSHIFT] ? flCameraMoveSpeedFast : flCameraMoveSpeed;
    if (key_states[SDL_SCANCODE_W])
	{
       	m_Position += (flMoveSpeed * deltaTime) * m_Forward;
    }

	if (key_states[SDL_SCANCODE_A]) 
	{
       	m_Position -= (flMoveSpeed * deltaTime) * m_Right;
    } 

	if (key_states[SDL_SCANCODE_S]) 
	{
       	m_Position -= (flMoveSpeed * deltaTime) * m_Forward;
    } 

	if (key_states[SDL_SCANCODE_D]) 
	{
       	m_Position += (flMoveSpeed * deltaTime) * m_Right;
    }

    if (key_states[SDL_SCANCODE_SPACE]) 
    {
        m_Position += (flMoveSpeed * deltaTime) * m_Up;
    }

    if (key_states[SDL_SCANCODE_LCTRL]) 
    {
        m_Position -= (flMoveSpeed * deltaTime) * m_Up;
    }

    //UpdateCameraAngles();
}

void CCamera::ProcessSDLMouseInput(SDL_Event& event, float deltaTime)
{
    switch(event.type)
    {
        case SDL_EVENT_MOUSE_MOTION:
        {

            float xoffset = event.motion.xrel;
	        xoffset *= flCameraSensivity;
	        float yoffset = -event.motion.yrel;
	        yoffset *= flCameraSensivity;

            m_Angles[1] += xoffset;
            m_Angles[0] += yoffset;


            if(m_Angles[0] > 89.0f)
                m_Angles[0] = 89.0f;
            if(m_Angles[0] < -89.0f)
                m_Angles[0] = -89.0f;


            /*
			float xoffset = event.motion.xrel;
			xoffset *= sensitivity;
			float yoffset = -event.motion.yrel;
			yoffset *= sensitivity;

					
    		yaw   += xoffset;
    		pitch += yoffset;


    		if(pitch > 89.0f)
    		    pitch = 89.0f;
    		if(pitch < -89.0f)
    		    pitch = -89.0f;
                */
			
            //UpdateCameraAngles();

    		glm::vec3 direction;
    		direction.x = cos(glm::radians(m_Angles[1])) * cos(glm::radians(m_Angles[0]));
    		direction.y = sin(glm::radians(m_Angles[0]));
    		direction.z = sin(glm::radians(m_Angles[1])) * cos(glm::radians(m_Angles[0]));
    		m_Forward = glm::normalize(direction);

            m_Right = glm::normalize(glm::cross(m_Forward, m_Up)); 

            break;
        }

        case SDL_EVENT_MOUSE_WHEEL:
        {
            SetFov(GetFov() + event.wheel.y);
            break;
        }
    }
}