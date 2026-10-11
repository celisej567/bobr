#pragma once

#include <vector>
#include "mytypes.h"
#include "vertex.hpp"

// CPU-only representations of loaded assets.
// The filesystem module knows nothing about the GPU, uploading is up to the user.

// For now this will be here.
// Need to move this.
struct texturedata_t
{
    int width = 0;
    int height = 0;
    int channels = 0;
    std::vector<uint8> pixels;
};

struct modeldata_t
{
    std::vector<vertex_t> verts;
    std::vector<uint> indexes;
};

inline bool IsValid(const texturedata_t& x)
{
    return x.width > 0 && x.height > 0 && x.channels > 0 && !x.pixels.empty();
}

inline bool IsValid(const modeldata_t& x)
{
    return !x.verts.empty() && !x.indexes.empty();
}
