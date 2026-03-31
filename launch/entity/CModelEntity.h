#pragma once
#include "shared.h"
#include "CEntity.h"
#include "CTexture.h"
#include "CModel.h"
#include "mytypes.h"

class CModelEntity : public CEntity
{
    BASECLASS(CModelEntity, CEntity);
public:
    CModelEntity();
    ~CModelEntity();

    void SetModelName(std::string filename);
    void SetTextureName(std::string filename, GLenum textureType);

    virtual void Think();

    virtual bool IsVisible() { return m_bVisible; };
    virtual void SetVisible(bool bEnable) { m_bVisible = bEnable; };

    virtual void Frame();

protected:

    bool m_bVisible = true;

private:
    CModel m_Model;

    CTexture m_Texture;
};