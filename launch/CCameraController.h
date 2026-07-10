#pragma once

#include "ICamera.h"
#include "inputmanager/InputManager.hpp"
#include "glm.hpp"

class CCameraController
{
public:
    CCameraController();
    CCameraController(ICamera* pCamera);

    // Call once per frame to process input and update the controlled camera
    // bProcessMouse — set to false to disable mouse look (e.g. when relative mouse is off)
    void Update(IInputManager* pInput, float deltaTime, bool bProcessMouse = true);

    // Set/get which camera this controller drives
    void SetCamera(ICamera* pCamera) { m_pCamera = pCamera; }
    ICamera* GetCamera() const { return m_pCamera; }

    // Movement speeds
    void SetMoveSpeed(float flNormalSpeed, float flFastSpeed);
    float GetMoveSpeedNormal() const { return m_flMoveSpeed; }
    float GetMoveSpeedFast() const { return m_flMoveSpeedFast; }

    // Mouse sensitivity
    void SetSensitivity(float flSensitivity) { m_flSensitivity = flSensitivity; }
    float GetSensitivity() const { return m_flSensitivity; }

    // Enable/disable processing
    void SetEnabled(bool bEnabled) { m_bEnabled = bEnabled; }
    bool IsEnabled() const { return m_bEnabled; }

private:
    void ProcessKeyboardInput(IInputManager* pInput, float deltaTime);
    void ProcessMouseInput(IInputManager* pInput);

    ICamera* m_pCamera = nullptr;

    float m_flMoveSpeed = 5.0f;
    float m_flMoveSpeedFast = 10.0f;
    float m_flSensitivity = 0.1f;

    bool m_bEnabled = true;
};