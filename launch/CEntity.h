#pragma once

#include "ECS.h"

class CEntity : public IEntity
{
public:
    virtual void Spawn();
    virtual void TickThink();
    virtual void Think();

protected:
    float m_flNextThink = 0;
};