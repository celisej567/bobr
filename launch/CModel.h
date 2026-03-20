#pragma once

#include "string"
#include "vector"
//#include "types.h"

struct vertex_t
{
	float x = 0;
	float y = 0;
	float z = 0;

	float r = 1;
	float g = 1;
	float b = 1;

	float u = 0;
	float v = 0;

    bool operator==(const vertex_t& other) const {
        return x == other.x && 
               y == other.y && 
               z == other.z && 
               r == other.r && 
               g == other.g && 
               b == other.b && 
               u == other.u &&
               v == other.v;
    }

};

template<> struct std::hash<vertex_t> {
    size_t operator()(const vertex_t& vertex) const noexcept {
        return (hash<float>()(vertex.x)) ^ ((hash<float>()(vertex.y)) << 1) ^ ((hash<float>()(vertex.z)) << 2) ^ ((hash<float>()(vertex.u)) << 3) ^ ((hash<float>()(vertex.v)) << 4);
    };
};

class CModel
{
public:
    uint VBO, VAO, EBO;

    CModel(){};

    // load model from file.
    CModel(std::string filename);

    // fill data with already existing arrays.
    CModel( vertex_t* verts, uint verts_size, uint* indexes, uint indexes_size );

    void ProcessFile( std::string filename );
    void ProcessData( vertex_t* verts, uint verts_size, uint* indexes, uint indexes_size );


    vertex_t* GetVerts() { return m_vecVerts.data(); };
    uint GetVertsCount() { return m_vecVerts.size(); };
    uint* GetIndexes()   { return m_vecIndexes.data(); };
    uint GetIndexesCount() { return m_vecIndexes.size(); };

private:

    void GenerateBuffers();

    std::vector<vertex_t> m_vecVerts;
    std::vector<uint>     m_vecIndexes;
};