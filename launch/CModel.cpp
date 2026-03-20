#include "CModel.h"
#include "string"
//#include "types.h"
#include "OBJ_Loader.h"
#include "glad/glad.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <unordered_map>

vertex_t vertices_EBO2[] = {
    {0.5f, 0.5f, 0, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},  // top right
    { 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},  // bottom right
    {-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},  // bottom left
    {-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f}   // top left 
};

unsigned int indices_EBO2[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

objl::Loader* g_objLoader = NULL;

CModel::CModel( std::string filename )
{
    ProcessFile(filename);
}

void CModel::ProcessFile( std::string filename )
{
    if(!g_objLoader)
        g_objLoader = new objl::Loader();

        /*
    bool bLoaded = g_objLoader->LoadFile(filename.data());

    if(!bLoaded) 
        std::abort();

    for (int i = 0; i < g_objLoader->LoadedMeshes.size(); i++)
	{
        objl::Mesh curMesh = g_objLoader->LoadedMeshes[i];

        
        for (int j = 0; j < curMesh.Vertices.size(); j++)
        {
            vertex_t vert;
            vert.x = curMesh.Vertices[j].Position.X;
            vert.y = curMesh.Vertices[j].Position.Y;
            vert.z = curMesh.Vertices[j].Position.Z;

            vert.u = curMesh.Vertices[j].TextureCoordinate.X;
            vert.v = curMesh.Vertices[j].TextureCoordinate.Y;

            m_vecVerts.push_back(vert);
        }

        for (int j = 0; j < curMesh.Indices.size(); j += 3)
		{
			m_vecIndexes.push_back(curMesh.Indices[j] - 1   );
			//m_vecIndexes.push_back(curMesh.Indices[j+1] - 1 );
			//m_vecIndexes.push_back(curMesh.Indices[j+2] - 1 );
		}

        //skip other ones
        break;
    }*/

    const std::string MODEL_PATH = filename;
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str())) {
        std::abort();
    }

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
                uniqueVertices[vertex] = (m_vecVerts.size());
                m_vecVerts.push_back(vertex);
            }
            

            m_vecIndexes.push_back(uniqueVertices[vertex]);
        }
    }

    GenerateBuffers();
}

void CModel::ProcessData( vertex_t* verts, uint verts_size, uint* indexes, uint indexes_size )
{
    m_vecVerts = std::vector<vertex_t>(verts, verts+(verts_size));
    m_vecIndexes = std::vector<uint>(indexes, indexes+(indexes_size));

    GenerateBuffers();
}

CModel::CModel( vertex_t* verts, uint verts_size, uint* indexes, uint indexes_size )
{
    ProcessData(verts, verts_size, indexes, indexes_size);
}


void CModel::GenerateBuffers()
{
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * GetVertsCount(), GetVerts(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * GetIndexesCount(), GetIndexes(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}