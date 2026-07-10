#include "CCameraController.h"

CCameraController::CCameraController()
{
}

CCameraController::CCameraController(ICamera* pCamera)
    : m_pCamera(pCamera)
{
}

void CCameraController::Update(IInputManager* pInput, float deltaTime, bool bProcessMouse)
{
    if (!m_bEnabled || !m_pCamera)
        return;

    ProcessKeyboardInput(pInput, deltaTime);
    if (bProcessMouse)
        ProcessMouseInput(pInput);
}

void CCameraController::SetMoveSpeed(float flNormalSpeed, float flFastSpeed)
{
    m_flMoveSpeed = flNormalSpeed;
    m_flMoveSpeedFast = flFastSpeed;
}

void CCameraController::ProcessKeyboardInput(IInputManager* pInput, float deltaTime)
{
    glm::vec3 pos = m_pCamera->GetPosition();
    float flMoveSpeed = pInput->IsKeyDown(KeyCode::Lshift) ? m_flMoveSpeedFast : m_flMoveSpeed;

    if (pInput->IsKeyDown(KeyCode::W))
        pos += (flMoveSpeed * deltaTime) * m_pCamera->GetForward();

    if (pInput->IsKeyDown(KeyCode::S))
        pos -= (flMoveSpeed * deltaTime) * m_pCamera->GetForward();

    if (pInput->IsKeyDown(KeyCode::A))
        pos -= (flMoveSpeed * deltaTime) * m_pCamera->GetRight();

    if (pInput->IsKeyDown(KeyCode::D))
        pos += (flMoveSpeed * deltaTime) * m_pCamera->GetRight();

    if (pInput->IsKeyDown(KeyCode::Space))
        pos += (flMoveSpeed * deltaTime) * m_pCamera->GetUp();

    if (pInput->IsKeyDown(KeyCode::Lctrl))
        pos -= (flMoveSpeed * deltaTime) * m_pCamera->GetUp();

    m_pCamera->SetPosition(pos);
}

void CCameraController::ProcessMouseInput(IInputManager* pInput)
{
    // Mouse look — only update angles and call SetAngles if there's actual mouse movement
    float xoffset = pInput->GetMouseDeltaX() * m_flSensitivity;
    float yoffset = -pInput->GetMouseDeltaY() * m_flSensitivity;

    bool bMouseMoved = (xoffset != 0.0f || yoffset != 0.0f);

    // Scroll wheel FOV
    float scrollDelta = pInput->GetScrollDelta();
    bool bScrolled = (scrollDelta != 0.0f);

    if (!bMouseMoved && !bScrolled)
        return;

    glm::vec3 angles = m_pCamera->GetAngles();

    if (bMouseMoved)
    {
        angles[1] += xoffset;
        angles[0] += yoffset;

        if (angles[0] > 89.0f)
            angles[0] = 89.0f;
        if (angles[0] < -89.0f)
            angles[0] = -89.0f;
    }

    if (bScrolled)
    {
        float fov = m_pCamera->GetFov();
        fov += scrollDelta;
        m_pCamera->SetFov(fov);
    }

    m_pCamera->SetAngles(angles);
}