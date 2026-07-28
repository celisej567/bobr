#pragma once
#include "ext/vector_float3.hpp"
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

    void SetModelName(const std::string &filename);
    void SetTextureName(const std::string &filename, GLenum textureType);
    void SetScale(float fX, float fY, float fZ);

    virtual void Think();

    virtual bool IsVisible() { return m_bVisible && m_Model.IsValid(); };
    virtual void SetVisible(bool bEnable) { m_bVisible = bEnable; };

    virtual void Frame();

protected:

    bool m_bVisible = true;

private:
    CModel m_Model;

    glm::vec3 m_vecScale;

    CTexture m_Texture;
};