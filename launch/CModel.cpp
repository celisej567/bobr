#include "CModel.h"
#include "string"
//#include "types.h"
#include "glad/glad.h"
#include <unordered_map>
#include "assetcache/assetcache.hpp"

CModel::CModel( std::string filename )
{
    ProcessFile(filename);
}

CModel::CModel( vertex_t* verts, uint verts_size, uint* indexes, uint indexes_size )
{
    ProcessData(verts, verts_size, indexes, indexes_size);
}

void CModel::ProcessFile( std::string filename )
{
    m_ModelCache = AssetCache::GetModelData(filename);
}

void CModel::ProcessData( vertex_t* verts, uint verts_size, uint* indexes, uint indexes_size )
{
     m_ModelCache = AssetCache::BuildModelCache(verts, verts_size, indexes, indexes_size);
}
