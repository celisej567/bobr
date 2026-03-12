#include "CShader.h"


unsigned int CompileShader(const char* buff, GLenum type)
{
    unsigned int uShader;
    uShader = glCreateShader(type);
    glShaderSource(uShader, 1, &buff, NULL);
    glCompileShader(uShader);

    {
        int  success;
        char infoLog[512];
        glGetShaderiv(uShader, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            glGetShaderInfoLog(uShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    return uShader;
}


CShader::CShader(const char* fragmentPath, const char* vertexPath)
{
    std::string vertexShader = GetFileString(vertexPath);
    std::string fragmentShader = GetFileString(fragmentPath);
    
    if( vertexShader.empty() || fragmentShader.empty() )
        std::exit(-1);

    vertShaderID = CompileShader(vertexShader.c_str(), GL_VERTEX_SHADER);
    fragShaderID = CompileShader(fragmentShader.c_str(), GL_FRAGMENT_SHADER);
    ID = glCreateProgram();
    glAttachShader(ID, fragShaderID);
    glAttachShader(ID, vertShaderID);
    glLinkProgram(ID);

    {
        int  success;
        char infoLog[512];
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADERPROGRAM::LINK::FAILED\n" << infoLog << std::endl;
        }
    }
    
    //delete[] vertexShader;
    //delete[] fragmentShader;


}

CShader::~CShader()
{
    //glDeleteShader(vertShaderID);
    //glDeleteShader(fragShaderID);
}