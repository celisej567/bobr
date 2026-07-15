#pragma once

#include "vector"

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

// usually used for determining unique verticies.
template<> struct std::hash<vertex_t> {
    size_t operator()(const vertex_t& vertex) const noexcept {
        return (hash<float>()(vertex.x)) ^ ((hash<float>()(vertex.y)) << 1) ^ ((hash<float>()(vertex.z)) << 2) ^ ((hash<float>()(vertex.u)) << 3) ^ ((hash<float>()(vertex.v)) << 4);
    };
};
