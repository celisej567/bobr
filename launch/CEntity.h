#pragma once

#include "ECS.h"

class CEntity : public IEntity
{
public:
    virtual void Spawn();
    virtual void Tick();
    virtual void Think();
    virtual void Frame();

    virtual bool IsEnabled() { return m_bEnabled; };
    virtual bool IsMarkedForDelete() { return m_bMarkForDelete; };

    virtual bool IsVisible() { return false; };
    virtual void SetVisible(bool bEnable) {};

    virtual void Enable() { m_bEnabled=true; };
    virtual void Disable() { m_bEnabled=false; };
    virtual void MarkForDelete() {m_bMarkForDelete = true; };

    virtual glm::vec3 GetAbsPos() { return m_AbsPos; };
    virtual glm::vec3 GetAbsRot() { return m_AbsRot; };

    virtual void SetAbsPos(glm::vec3 vec) { m_AbsPos = vec; };
    virtual void SetAbsRot(glm::vec3 vec) { m_AbsRot = vec; };

    virtual uint GetEntityIndex() { return m_uEntIndex; };
    virtual std::string GetClassname() { return m_strClassname; };

protected:

    virtual void SetEntityIndex(uint index) { m_uEntIndex = index; };
    virtual void SetClassname(std::string classname) { m_strClassname = classname; };

protected:
    float m_flNextThink = 0;
    bool m_bEnabled = false;
    bool m_bMarkForDelete = false;

    glm::vec3 m_AbsPos = {0,0,0};
    glm::vec3 m_AbsRot = {0,0,0};

    uint m_uEntIndex = ENTITY_INVALID_INDEX;
    std::string m_strClassname;
};