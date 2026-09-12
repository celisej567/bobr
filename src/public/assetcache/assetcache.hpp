#pragma once
#include "modelcache.hpp"
#include "texturecache.hpp"
#include "unordered_map"
#include "string"
#include <cstdint>

class AssetCache
{
public:
    static const modelcache_t& GetModelData( const std::string &strModelPath );
    static const texturecache_t& GetTextureData( const std::string &strTexturePath );

    static const modelcache_t& BuildModelCache( vertex_t* verts, uint verts_size, uint* indexes, uint indexes_size );

    static const modelcache_t& GetEmptyModelCache() {return s_EmptyModelCache;};
    static const texturecache_t& GetEmptyTextureCache() {return s_EmptyTextureCache;};

    static void Destroy();

private:
    static const modelcache_t s_EmptyModelCache;   
    static const texturecache_t s_EmptyTextureCache;   

    static bool LoadModelFromDisk(const std::string &strModelPath);
    static bool LoadTextureFromDisk(const std::string &strTexturePath);
};