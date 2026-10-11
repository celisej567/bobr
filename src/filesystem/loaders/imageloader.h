#pragma once

#include "filesystem/filedata.h"
#include <cstddef>

// Decodes an image file that is already in memory.
bool LoadImageFromMemory(const uint8* pData, size_t iSize, texturedata_t& out);
