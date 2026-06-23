#pragma once
#include "glad/glad.h"
//#include "GL/gl.h"
#include "mytypes.h"
#include "assetcache/assetcache.hpp"
#include "texturecache.hpp"

class CTexture
{
public:

    CTexture(){};
    CTexture(const char* filename, GLenum textureType);
    void ProcessFile(const char* filename, GLenum textureType);
    virtual ~CTexture();

    bool IsValid() {return ::IsValid(m_TextureCache);};
    void GetResolution(int& x, int& y ) 
    {
        if(::IsValid(m_TextureCache))
        {
            x = m_TextureCache->width;
            y = m_TextureCache->height;
            return;
        }

        x = 0;
        y = 0;
    }

    GLenum GetTextureType() { return ::IsValid(m_TextureCache) ? m_TextureCache->textureType : 0 ;}

    operator uint() const
    {
        return m_TextureCache->ID;
    }

private:
    const texturecache_t* m_TextureCache = &AssetCache::GetEmptyTextureCache();

};