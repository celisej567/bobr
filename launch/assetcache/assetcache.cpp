#include "assetcache/assetcache.hpp"
#include "modelcache.hpp"
#include "mytypes.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "iostream"

#include "glad/glad.h"

static std::unordered_map<std::string, modelcache_t> m_mapCachedModels;

const modelcache_t AssetCache::s_EmptyCache;

const modelcache_t& AssetCache::GetModelData( std::string strModelPath )
{
    if( m_mapCachedModels.contains(strModelPath) )
        return m_mapCachedModels[strModelPath];

    if(LoadModelFromDisk(strModelPath))
        return m_mapCachedModels[strModelPath];

    return GetEmptyModelCache();
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

    // now we need to make VBO
    // TODO(celisej): i think its better to move this somewhere else

    glGenVertexArrays(1, &(mdlcache.VAO));
    glGenBuffers(1, &(mdlcache.VBO));
    glGenBuffers(1, &(mdlcache.EBO));

    glBindVertexArray(mdlcache.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mdlcache.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * mdlcache.m_vecVerts.size(), mdlcache.m_vecVerts.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mdlcache.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mdlcache.m_vecIndexes.size(), mdlcache.m_vecIndexes.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // mark cache as filled with valid data.
    mdlcache.bInit = true;

    return m_mapCachedModels[strKey];
}

bool AssetCache::LoadModelFromDisk(std::string strModelPath)
{
    const std::string MODEL_PATH = strModelPath;
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str())) 
    {
        std::cout << err << std::endl;
        return false;
    }

    modelcache_t mdlcache;

    // firstly we need to fill up verts and index vectors
    std::unordered_map<vertex_t, uint> uniqueVertices {};
    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            vertex_t vertex;

            if (index.vertex_index >= 0) {
                vertex.x = attrib.vertices[(3 * index.vertex_index) + 0];
                vertex.y = attrib.vertices[(3 * index.vertex_index) + 2];
                vertex.z = attrib.vertices[(3 * index.vertex_index) + 1];
            }

            if (index.texcoord_index >= 0) {
                vertex.u = attrib.texcoords[(2 * index.texcoord_index) + 0];
                vertex.v = 1.0f - attrib.texcoords[(2 * index.texcoord_index) + 1];
            } else {
                vertex.u = (vertex.x * 0.5f) + 0.5f;
                vertex.v = (vertex.z * 0.5f) + 0.5f;
            }

            
            if (!uniqueVertices.contains(vertex)) {
                uniqueVertices[vertex] = (mdlcache.m_vecVerts.size());
                mdlcache.m_vecVerts.push_back(vertex);
            }
            
            mdlcache.m_vecIndexes.push_back(uniqueVertices[vertex]);
        }
    }
    
    // mark cache as filled with valid data.
    mdlcache.bInit = true;

    m_mapCachedModels[strModelPath] = mdlcache;

    // now we need to make VBO
    // TODO(celisej): i think its better to move this somewhere else

    glGenVertexArrays(1, &(m_mapCachedModels[strModelPath].VAO));
    glGenBuffers(1, &(m_mapCachedModels[strModelPath].VBO));
    glGenBuffers(1, &(m_mapCachedModels[strModelPath].EBO));

    glBindVertexArray(m_mapCachedModels[strModelPath].VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_mapCachedModels[strModelPath].VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * m_mapCachedModels[strModelPath].m_vecVerts.size(), m_mapCachedModels[strModelPath].m_vecVerts.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mapCachedModels[strModelPath].EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m_mapCachedModels[strModelPath].m_vecIndexes.size(), m_mapCachedModels[strModelPath].m_vecIndexes.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    
    return true;

}