#pragma once 


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

// move this to dedicated module
class CBasicFileSystem : public IFileSystem
{
public:
    CBasicFileSystem();
    virtual ~CBasicFileSystem();

    virtual bool Initialize() override;
    virtual void Shutdown() override;

    virtual const char* GetModuleName() override {return "CBasicFileSystem";};

    virtual void ClearBasePaths() override;
    virtual std::string ReadFile(const std::string &strFilePath) override;

private:
    // TODO
    std::unordered_map<std::string, std::string> m_mapBasePaths;
};