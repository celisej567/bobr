#include "imageloader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool LoadImageFromMemory(const uint8* pData, size_t iSize, texturedata_t& out)
{
    // OpenGL expects the first row to be the bottom one.
    stbi_set_flip_vertically_on_load(1);

    int width = 0;
    int height = 0;
    int channels = 0;

    uint8* pPixels = stbi_load_from_memory(pData, static_cast<int>(iSize), &width, &height, &channels, 0);
    if(!pPixels)
        return false;

    out.width = width;
    out.height = height;
    out.channels = channels;
    out.pixels.assign(pPixels, pPixels + (static_cast<size_t>(width) * height * channels));

    stbi_image_free(pPixels);
    return true;
}
