#pragma once
#include "mytypes.h"
#include "CEntity.h"

class CGizmos : public CEntity
{
public:
    CGizmos();
    ~CGizmos();
    virtual bool IsVisible() { return m_bVisible; };
    virtual void SetVisible(bool bEnable) { m_bVisible = bEnable; };

    virtual void Frame();

protected:
    uint VAO;
    bool m_bVisible = true;
};