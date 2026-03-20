#pragma once
#include "FileSystemStuff.h"
#include "glad/glad.h"
//#include "GL/gl.h"
#include "string"
#include "unordered_map"

class CShader
{
public:
    // the program ID
    unsigned int ID;
    unsigned int vertShaderID;
    unsigned int fragShaderID;
  
    // constructor reads and builds the shader
    CShader(std::string  vertexPath, std::string fragmentPath);
    virtual ~CShader();

    // use/activate the shader
    void Use() 
    { 
        glUseProgram(ID);
    } 
    
    // utility uniform functions
    void SetUniformBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }

    void SetUniformInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }

    void SetUniformFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }

    void SetUniformVec3(const std::string &name, float value1, float value2, float value3) const
    { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3); 
    }

    void SetUniformVec4(const std::string &name, float value1, float value2, float value3, float value4) const
    { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4); 
    }

    void SetUniformMat4(const std::string &name, GLfloat* value, GLsizei count = 1, GLboolean transpose = GL_FALSE )
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), count, transpose, value);
    }
};

void CompileAllShaders();

unsigned int CompileShader(const char* buff, GLenum type);

inline std::unordered_map<std::string, CShader*> g_ShaderIDMap;