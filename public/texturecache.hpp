#pragma once
#include "mytypes.h"
#include "glad/glad.h"


struct texturecache_t
{
    uint ID = 0;
    int width = 0;
    int height = 0;
    GLenum textureType = 0;

    bool bInit = false;
    unsigned char *textureData = 0;
};


inline bool IsValid(const texturecache_t& x)
{
    if(!(x.bInit))
        return false;

    if (x.ID == 0)
        return false;

    if( x.width == 0 || x.height == 0 || x.textureType == 0)
        return false; 

    if (x.textureData == 0)        
        return false;

    return true; 
}

inline bool IsValid(const texturecache_t* x)
{
    if(!x)
        return false;
    
    if(!(x->bInit))
        return false;

    if (x->ID == 0)
        return false;

    if( x->width == 0 || x->height == 0 || x->textureType == 0)
        return false; 

    if (x->textureData == 0)        
        return false;
    
    return true; 
}