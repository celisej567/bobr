#pragma once

#include "filesystem/filedata.h"
#include <string>

// Parses .obj text that is already in memory.
// Materials are ignored.
bool LoadObjFromMemory(const std::string& strObj, modeldata_t& out, std::string& strError);
