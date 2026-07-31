#include "cbase.h"
#include "libs.h"
#include "filesystem/filesystem.h"
#include <cstdio>
#include <linux/limits.h>
#include <string>
#include <unordered_map>
#include "cmd/cmd.h"

#include "filesystem"
#include "fstream"
#include <cerrno>   // For errno
#include <cstring>  // For std::strerror
#include "basicfilesystem.h"

CBasicFileSystem::CBasicFileSystem()
{
    ClearBasePaths();

    m_mapBasePaths["ROOT"] = std::filesystem::current_path();
}

CBasicFileSystem::~CBasicFileSystem()
{

}

bool CBasicFileSystem::Initialize() 
{
    return true;
}

void CBasicFileSystem::Shutdown() 
{

}

void CBasicFileSystem::ClearBasePaths()
{
    m_mapBasePaths.clear();
}

std::string CBasicFileSystem::ReadFile(const std::string &strFilePath)
{

    // TODO: implement m_mapBasePaths

    if(strFilePath.empty() || !(strFilePath.length()) || IsAbsolutePath(strFilePath) )
    {
        CMD::Msg("Error ReadFile: path should not be empty or be absolute.\n");
        return std::string();
    }


    std::ifstream inFile(FixSlashesStdRet(strFilePath)/*, std::ios_base::ate*/);
    if (!inFile.is_open())
    {
        CMD::Msg("Error ReadFile: %s.\n", std::strerror(errno));
        return std::string();
    }

    std::stringstream ghghgh;

    ghghgh << inFile.rdbuf();

    return ghghgh.str();
}