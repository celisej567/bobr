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

    // should always be there
    m_mapBasePaths["root"] = std::filesystem::current_path();
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

void CBasicFileSystem::SetBasePath(const std::string & strBasePath, const std::string & strPath)
{
    if( strBasePath.empty() || strPath.empty())
        return;

    std::string strNewBasePath = ToLowerRet(strBasePath);
    std::string strNewPath = FixSlashesStdRet(strPath);

    if( !(strNewPath.ends_with('/')) )
        return;

    if( !(strNewPath[0] == '.') )
        strNewPath = Vars("./%s", strNewPath.c_str());

    m_mapBasePaths[strNewBasePath] = strNewPath;
}

std::string CBasicFileSystem::ReadFile(const std::string &strFilePath)
{

    // TODO: implement m_mapBasePaths

    if(strFilePath.empty() || !(strFilePath.length()))
    {
        CMD::Msg("Error ReadFile: path should not be empty or be absolute.\n");
        return std::string();
    }


    std::string strPath = FixSlashesStdRet(strFilePath);

    if(strFilePath[0] == '.')
        strPath = Vars("./%s", strPath.c_str());

    std::ifstream inFile(strPath/*, std::ios_base::ate*/);
    if (!inFile.is_open())
    {
        CMD::Msg("Error ReadFile: %s.\n", std::strerror(errno));
        return std::string();
    }

    std::stringstream ghghgh;

    ghghgh << inFile.rdbuf();

    return ghghgh.str();
}