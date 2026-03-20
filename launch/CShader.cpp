#include "CShader.h"

#include "string"
#include "vector"
#include "filesystem"


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


CShader::CShader(std::string fragmentPath, std::string vertexPath)
{
    std::string vertexShader = GetFileString(vertexPath);
    std::string fragmentShader = GetFileString(fragmentPath);
    
    if( vertexShader.empty() || fragmentShader.empty() )
        std::abort();

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


// rewrite this garbage
namespace fs = std::filesystem;
#include <ranges>
#include <string_view>
void CompileAllShaders()
{
    std::string path = "shaders";
    std::vector<std::string> shaderFiles;

    if (fs::exists(path) && fs::is_directory(path)) 
    {
        for (const auto& entry : fs::directory_iterator(path))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".shader") 
            {
                shaderFiles.push_back(entry.path().filename().string());
            }
        }
    }

    /// do some actual parsing here later
    for( std::string strFile : shaderFiles )
    {
        std::unordered_map<GLenum, std::string> Shaders;
        std::string buff = GetFileString(path+"/"+strFile);

        size_t lastindex = strFile.find_last_of("."); 
        std::string rawshadername = strFile.substr(0, lastindex);

        std::vector<std::string> lines = MySplitString(buff);

        for(std::string line : lines )
        {
            size_t lastindex1 = line.find_last_of("."); 
            std::string filetype = line.substr(lastindex1, line.size()-1);

            GLenum type = GL_VERTEX_SHADER;

            if( filetype.find("vert") != std::string::npos )
            {
                type = GL_VERTEX_SHADER;
            }
            else if( filetype.find("frag") != std::string::npos )
            {
                type = GL_FRAGMENT_SHADER;
            }

            Shaders[type] = line;
        }

        g_ShaderIDMap[rawshadername] = new CShader( path+"/"+Shaders[GL_FRAGMENT_SHADER], path+"/"+Shaders[GL_VERTEX_SHADER]);
        
    }
}