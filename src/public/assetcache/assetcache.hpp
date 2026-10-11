#pragma once
#include "modelcache.hpp"
#include "texturecache.hpp"
#include "unordered_map"
#include "string"
#include <cstdint>

class AssetCache
{
public:
    // Empty basePath means "root".
    // Cache key is the path in lower case, base path is not a part of it.
    static const modelcache_t& GetModelData( const std::string &strModelPath, const std::string &strBasePath = "" );
    static const texturecache_t& GetTextureData( const std::string &strTexturePath, const std::string &strBasePath = "" );

    // its better to never use this one
    static const modelcache_t& BuildModelCache( vertex_t* verts, uint verts_size, uint* indexes, uint indexes_size );

    static const modelcache_t& GetEmptyModelCache() {return s_EmptyModelCache;};
    static const texturecache_t& GetEmptyTextureCache() {return s_EmptyTextureCache;};

    static const texturecache_t GenerateErrorTexture();

    static void Destroy();

private:
    static const modelcache_t s_EmptyModelCache;
    static const texturecache_t s_EmptyTextureCache;

    static bool LoadModelFromDisk(const std::string &strModelPath, const std::string &strBasePath);
    static bool LoadTextureFromDisk(const std::string &strTexturePath, const std::string &strBasePath);

    static void UploadModel(modelcache_t &mdlcache);
    static void UploadTexture(texturecache_t &texcache, uint nrChannels, const unsigned char *pixels);
};
