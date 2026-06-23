#pragma once
#include "mytypes.h"
#include "vertex.hpp"

#define MODELCACHE_VBO_INVALID_VALUE 0

// TODO(celisej): make similar stuff to support shapes, materials, etc...
struct modelcache_t
{
    bool bInit = false;
    uint VBO = MODELCACHE_VBO_INVALID_VALUE;
    uint VAO = MODELCACHE_VBO_INVALID_VALUE;
    uint EBO = MODELCACHE_VBO_INVALID_VALUE;

    std::vector<vertex_t> m_vecVerts;
    std::vector<uint>     m_vecIndexes;
};

inline bool IsValid(const modelcache_t& x)
{
    if(!(x.bInit))
        return false;

    if (x.VBO == MODELCACHE_VBO_INVALID_VALUE ||
        x.VAO == MODELCACHE_VBO_INVALID_VALUE ||
        x.EBO == MODELCACHE_VBO_INVALID_VALUE
    )
        return false;

    if( x.m_vecVerts.empty() || x.m_vecIndexes.empty() )
        return false; 

    return true; 
}

inline bool IsValid(const modelcache_t* x)
{
    if(!(x->bInit))
        return false;

    if (x->VBO == MODELCACHE_VBO_INVALID_VALUE ||
        x->VAO == MODELCACHE_VBO_INVALID_VALUE ||
        x->EBO == MODELCACHE_VBO_INVALID_VALUE
    )
        return false;

    if( x->m_vecVerts.empty() || x->m_vecIndexes.empty() )
        return false; 

    return true; 
}