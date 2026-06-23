#include "iostream"
#include "CTexture.h"
#include "stb_image.h"
#include "FileSystemStuff.h"
#include "glad/glad.h"
//#include "GL/gl.h"

#include "shared.h"
#include "texturecache.hpp"
#include "assetcache/assetcache.hpp"


CTexture::CTexture(const char* filename, GLenum textureType)
{
    ProcessFile(filename,textureType);
}

CTexture::~CTexture()
{
    if(::IsValid(m_TextureCache) && wnd)
        glDeleteTextures(1, &(m_TextureCache->ID));
}

void CTexture::ProcessFile(const char* filename, GLenum textureType)
{
    if(!filename || !filename[0])
        return;

    m_TextureCache = &AssetCache::GetTextureData(filename);
        
}