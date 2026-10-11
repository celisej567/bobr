#include "objloader.h"

#include <sstream>
#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

bool LoadObjFromMemory(const std::string& strObj, modeldata_t& out, std::string& strError)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;

    std::istringstream stream(strObj);

    // no material reader, so mtllib lines are skipped.
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &strError, &stream, nullptr))
        return false;

    size_t totalIndices = 0;
    for (const auto& shape : shapes)
        totalIndices += shape.mesh.indices.size();

    std::unordered_map<vertex_t, uint> uniqueVertices;
    uniqueVertices.reserve(totalIndices);

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            vertex_t vertex;

            // Y and Z are swapped on purpose.
            if (index.vertex_index >= 0) {
                vertex.x = attrib.vertices[(3 * index.vertex_index) + 0];
                vertex.y = attrib.vertices[(3 * index.vertex_index) + 2];
                vertex.z = attrib.vertices[(3 * index.vertex_index) + 1];
            }

            if (index.normal_index >= 0) {
                vertex.nx = attrib.normals[(3 * index.normal_index) + 0];
                vertex.ny = attrib.normals[(3 * index.normal_index) + 2];
                vertex.nz = attrib.normals[(3 * index.normal_index) + 1];
            }

            if (index.texcoord_index >= 0) {
                vertex.u = attrib.texcoords[(2 * index.texcoord_index) + 0];
                vertex.v = 1.0f - attrib.texcoords[(2 * index.texcoord_index) + 1];
            } else {
                vertex.u = (vertex.x * 0.5f) + 0.5f;
                vertex.v = (vertex.z * 0.5f) + 0.5f;
            }

            auto it = uniqueVertices.find(vertex);
            if (it == uniqueVertices.end()) {
                it = uniqueVertices.emplace(vertex, static_cast<uint>(out.verts.size())).first;
                out.verts.push_back(vertex);
            }

            out.indexes.push_back(it->second);
        }
    }

    return true;
}
