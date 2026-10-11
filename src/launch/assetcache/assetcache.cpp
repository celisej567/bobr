#include "assetcache/assetcache.hpp"
#include "modelcache.hpp"
#include "profiler.hpp"
#include "texturecache.hpp"
#include "mytypes.h"

#include "manyinterfaces.h"
#include "platform.h"
#include "iostream"

#include "glad/glad.h"

static std::unordered_map<std::string, modelcache_t> m_mapCachedModels;
static std::unordered_map<std::string, texturecache_t> m_mapCachedTextures;

const modelcache_t AssetCache::s_EmptyModelCache;
const texturecache_t AssetCache::s_EmptyTextureCache;

static const unsigned char errTexData[16] =
{
    0, 0, 0,     105, 0, 198,   0, 0,
    105, 0, 198, 0, 0, 0,       0, 0
};

void AssetCache::Destroy()
{
    for (auto it = m_mapCachedModels.begin(); it != m_mapCachedModels.end(); )
    {
        modelcache_t& mdlcache = it->second;

        // just to be safe
        if(IsValid(mdlcache))
        {
            glDeleteBuffers(1, &(mdlcache.EBO));
            glDeleteBuffers(1, &(mdlcache.VBO));
            glDeleteVertexArrays(1, &(mdlcache.VAO));

            it = m_mapCachedModels.erase(it);
            continue;
        }
        ++it;
    }

    for (auto it = m_mapCachedTextures.begin(); it != m_mapCachedTextures.end(); )
    {
        texturecache_t& texcache = it->second;

        // just to be safe
        if(IsValid(texcache))
        {
            glDeleteTextures(1, &(texcache.ID));
            it = m_mapCachedTextures.erase(it);
            continue;
        }
        ++it;
    }
}

const texturecache_t AssetCache::GenerateErrorTexture()
{

    texturecache_t texcache_new;
    texcache_new.bInit = true;

    GLuint textureTypeInternal = GL_RGB8;
    GLuint textureType = GL_RGB;

    texcache_new.textureType = textureTypeInternal;
    texcache_new.width = 2;
    texcache_new.height = 2;

    glGenTextures(1, &texcache_new.ID);
    glBindTexture(GL_TEXTURE_2D, texcache_new.ID);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    glTexImage2D(GL_TEXTURE_2D, 0, textureTypeInternal, texcache_new.width, texcache_new.height, 0, textureType, GL_UNSIGNED_BYTE, errTexData);

    return texcache_new;
}

const modelcache_t& AssetCache::GetModelData( const std::string &strModelPath, const std::string &strBasePath )
{
    std::string strKey = ToLowerRet(strModelPath);

    if( m_mapCachedModels.contains(strKey) )
        return m_mapCachedModels[strKey];

    if(LoadModelFromDisk(strModelPath, strBasePath))
        return m_mapCachedModels[strKey];

    return GetEmptyModelCache();
}

const texturecache_t& AssetCache::GetTextureData( const std::string &strTexturePath, const std::string &strBasePath )
{
    std::string strKey = ToLowerRet(strTexturePath);

    if( m_mapCachedTextures.contains(strKey) )
        return m_mapCachedTextures[strKey];

    if(LoadTextureFromDisk(strTexturePath, strBasePath))
        return m_mapCachedTextures[strKey];

    return GetEmptyTextureCache();
}

static inline uint64_t FNV1aHashBytes( const char* data, size_t size )
{
    uint64_t hash = 14695981039346656037ULL; // FNV offset basis
    for( size_t i = 0; i < size; ++i )
    {
        hash ^= static_cast<uint8_t>(data[i]);
        hash *= 1099511628211ULL; // FNV prime
    }
    return hash;
}

const modelcache_t& AssetCache::BuildModelCache( vertex_t* verts, uint verts_size, uint* indexes, uint indexes_size )
{
    uint64_t hashVerts = FNV1aHashBytes( reinterpret_cast<const char*>(verts), verts_size * sizeof(vertex_t) );
    uint64_t hashIdx   = FNV1aHashBytes( reinterpret_cast<const char*>(indexes), indexes_size * sizeof(uint) );
    std::string strKey = "@built_FNVhash:(" + std::to_string(hashVerts) + "):(" + std::to_string(hashIdx) + ")";

    // If this exact mesh data was already built, return cached version
    if( m_mapCachedModels.contains(strKey) )
        return m_mapCachedModels[strKey];

    modelcache_t& mdlcache = m_mapCachedModels[strKey];
    mdlcache.m_vecVerts = std::vector<vertex_t>(verts, verts+(verts_size));
    mdlcache.m_vecIndexes = std::vector<uint>(indexes, indexes+(indexes_size));

    UploadModel(mdlcache);

    return m_mapCachedModels[strKey];
}

void AssetCache::UploadModel(modelcache_t &mdlcache)
{
    PROFILER_SCOPE_NAME("Bind Buffers");

    // now we need to make VBO

    glGenVertexArrays(1, &(mdlcache.VAO));
    glGenBuffers(1, &(mdlcache.VBO));
    glGenBuffers(1, &(mdlcache.EBO));

    glBindVertexArray(mdlcache.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mdlcache.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * mdlcache.m_vecVerts.size(), mdlcache.m_vecVerts.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mdlcache.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mdlcache.m_vecIndexes.size(), mdlcache.m_vecIndexes.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, x));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, r));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, u));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, sizeof(vertex_t), (void*)offsetof(vertex_t, nx));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // mark cache as filled with valid data.
    mdlcache.bInit = true;
}

bool AssetCache::LoadModelFromDisk(const std::string &strModelPath, const std::string &strBasePath)
{
    PROFILER_SCOPE_NAME("LoadModelFromDisk")
    PROFILER_SCOPE_TEXT(strModelPath.data(), strModelPath.length());

    // i dunno how to feel about passing strBasePath thru here
    const modeldata_t &model = g_pFileSystem->GetModelData(strBasePath, strModelPath);
    if(!IsValid(model))
        return false;

    modelcache_t mdlcache;
    mdlcache.m_vecVerts = model.verts;
    mdlcache.m_vecIndexes = model.indexes;

    UploadModel(mdlcache);

    m_mapCachedModels[ToLowerRet(strModelPath)] = std::move(mdlcache);
    return true;
}

void AssetCache::UploadTexture(texturecache_t &texcache, uint nrChannels, const unsigned char *pixels)
{
    std::cout << "Channels: " << nrChannels << "\n";

    GLenum textureType;
    GLenum textureTypeInternal;

    switch(nrChannels)
    {
        case 1:
            textureTypeInternal = GL_R8;
            textureType = GL_RED;
            std::cout << "Type: GL_RED\n";
            std::cout << "Internal Type: GL_R8\n";
            break;

        case 2:
            textureTypeInternal = GL_RG8;
            textureType = GL_RG;
            std::cout << "Type: GL_RG\n";
            std::cout << "Internal Type: GL_RG8\n";
            break;

        case 3:
            textureTypeInternal = GL_RGB8;
            textureType = GL_RGB;
            std::cout << "Type: GL_RGB\n";
            std::cout << "Internal Type: GL_RGB8\n";
            break;

        case 4:
            textureTypeInternal = GL_RGBA8;
            textureType = GL_RGBA;
            std::cout << "Type: GL_RGBA\n";
            std::cout << "Internal Type: GL_RGBA8\n";
            break;

        default:
            textureTypeInternal = GL_RGB8;
            textureType = GL_RGB;
            std::cout << "Type: GL_RGB\n";
            std::cout << "Internal Type: GL_RGB8\n";
            break;
    }

    texcache.textureType = textureTypeInternal;

    glGenTextures(1, &texcache.ID);
    glBindTexture(GL_TEXTURE_2D, texcache.ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, textureTypeInternal, texcache.width, texcache.height, 0, textureType, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
}

bool AssetCache::LoadTextureFromDisk(const std::string &strTexturePath, const std::string &strBasePath)
{
    PROFILER_SCOPE_NAME("LoadTextureFromDisk")
    PROFILER_SCOPE_TEXT(strTexturePath.data(), strTexturePath.length());

    std::string strKey = ToLowerRet(strTexturePath);

    // i dunno how to feel about passing strBasePath thru here
    const texturedata_t &texture = g_pFileSystem->GetTextureData(strBasePath, strTexturePath);

    if(!IsValid(texture))
    {
        std::cout << "Failed to load texture " << strTexturePath << std::endl;

        m_mapCachedTextures[strKey] = GenerateErrorTexture();

        return true;
    }

    std::cout << "Texture: " << strTexturePath << "\n";

    texturecache_t texcache;
    texcache.width = texture.width;
    texcache.height = texture.height;
    texcache.bInit = true;

    UploadTexture(texcache, texture.channels, texture.pixels.data());

    m_mapCachedTextures[strKey] = texcache;
    return true;
}
