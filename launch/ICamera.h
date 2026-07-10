#pragma once 

#include "glm.hpp"
#include "mytypes.h"

class ICamera
{
public:
    virtual glm::mat4 GetViewMatrix() = 0;
    virtual float GetFov() = 0;
    virtual void SetFov(float flFov) = 0;

    virtual glm::vec3 GetPosition() const = 0;
    virtual void SetPosition(const glm::vec3& pos) = 0;

    virtual glm::vec3 GetAngles() const = 0;
    virtual void SetAngles(const glm::vec3& angles) = 0;

    virtual glm::vec3 GetForward() const = 0;
    virtual glm::vec3 GetRight() const = 0;
    virtual glm::vec3 GetUp() const = 0;
};
