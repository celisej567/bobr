#pragma once
#include "modelcache.hpp"
#include "unordered_map"
#include "string"
#include <cstdint>

class AssetCache
{
public:
    static const modelcache_t& GetModelData( std::string strModelPath );

    static const modelcache_t& BuildModelCache( vertex_t* verts, uint verts_size, uint* indexes, uint indexes_size );

    static const modelcache_t& GetEmptyModelCache() {return s_EmptyCache;};
private:
    static const modelcache_t s_EmptyCache;   

    static bool LoadModelFromDisk(std::string strModelPath);
};