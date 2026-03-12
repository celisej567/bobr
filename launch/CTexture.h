#pragma once
#include "glad/glad.h"
//#include "GL/gl.h"

class CTexture
{
public:
    unsigned int ID;
    int width, height;

    CTexture(const char* filename, GLenum textureType);
    ~CTexture();

    operator unsigned int() const
    {
        return ID;
    }

};