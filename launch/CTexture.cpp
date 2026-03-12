#include "iostream"
#include "CTexture.h"
#include "stb_image.h"
#include "FileSystemStuff.h"
#include "glad/glad.h"
//#include "GL/gl.h"




#include <ctype.h>

char *stristr(const char *String, const char *Pattern)
{
      char *pptr, *sptr, *start;

      for (start = (char *)String; *start; start++)
      {
            /* find start of pattern in string */
            for ( ; (*start && (toupper(*start) != toupper(*Pattern))); start++)
                  ;
            if (!*start)
                  return 0;

            pptr = (char *)Pattern;
            sptr = (char *)start;

            while (toupper(*sptr) == toupper(*pptr))
            {
                  sptr++;
                  pptr++;

                  /* if end of pattern then pattern was found */

                  if (!*pptr)
                        return (start);
            }
      }
      return 0;
}


CTexture::CTexture(const char* filename, GLenum textureType)
{
    if(!filename || !filename[0])
        std::exit(-1);

    int nrChannels;
    unsigned char *textureData = stbi_load(filename, &width, &height, &nrChannels, 0);

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (textureData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, textureType, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << filename << std::endl;
        std::exit(-1);
    }
    stbi_image_free(textureData);
}

CTexture::~CTexture()
{
    glDeleteTextures(1, &ID);
}