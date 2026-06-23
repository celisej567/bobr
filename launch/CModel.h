#pragma once

#include "assetcache/assetcache.hpp"
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

    const modelcache_t* GetModelCache() const {return m_ModelCache;};
    bool IsValid() {return m_ModelCache && ::IsValid(*m_ModelCache);}

    const vertex_t* GetVerts() const { return m_ModelCache ? m_ModelCache->m_vecVerts.data() : nullptr; };
    uint GetVertsCount() const { return m_ModelCache ? m_ModelCache->m_vecVerts.size() : 0; };
    const uint* GetIndexes() const { return m_ModelCache ? m_ModelCache->m_vecIndexes.data() : nullptr; };
    uint GetIndexesCount() const { return m_ModelCache ? m_ModelCache->m_vecIndexes.size() : 0; };

private:
    const modelcache_t* m_ModelCache = &AssetCache::GetEmptyModelCache();
};