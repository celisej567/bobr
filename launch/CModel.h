#pragma once

#include "modelcache.hpp"
#include "string"
#include "vector"
#include "mytypes.h"
#include "vertex.hpp"
#include "modelcache.hpp"

class CModel
{
public:

    CModel(){};

    // load model from file.
    CModel(std::string filename);

    // fill data with already existing arrays.
    CModel( vertex_t* verts, uint verts_size, uint* indexes, uint indexes_size );

    void ProcessFile( std::string filename );

    void ProcessData( vertex_t* verts, uint verts_size, uint* indexes, uint indexes_size );

    modelcache_t GetModelCache() {return m_ModelCache;};
    bool IsValid() {return ::IsValid(m_ModelCache);}

    vertex_t* GetVerts() { return m_ModelCache.m_vecVerts.data(); };
    uint GetVertsCount() { return m_ModelCache.m_vecVerts.size(); };
    uint* GetIndexes()   { return m_ModelCache.m_vecIndexes.data(); };
    uint GetIndexesCount() { return m_ModelCache.m_vecIndexes.size(); };

private:
    modelcache_t m_ModelCache;
};