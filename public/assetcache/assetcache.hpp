#pragma once
#include "modelcache.hpp"
#include "unordered_map"
#include "string"
#include <cstdint>

class AssetCache
{
public:
    static modelcache_t GetModelData( std::string strModelPath );

    static modelcache_t BuildModelCache( vertex_t* verts, uint verts_size, uint* indexes, uint indexes_size );


private:

    static bool LoadModelFromDisk(std::string strModelPath);
};