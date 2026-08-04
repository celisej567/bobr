#pragma once
#include "mytypes.h"
#include "CEntity.h"

class CAxis : public CEntity
{
public:
    CAxis();
    ~CAxis();
    virtual bool IsVisible() { return m_bVisible; };
    virtual void SetVisible(bool bEnable) { m_bVisible = bEnable; };

    virtual void Frame();

protected:
    uint VAO;
    bool m_bVisible = true;
};