#pragma once

#include "libs.h"
#include "string"

class IFileSystem : public IModuleBase
{
public:

    virtual const char* GetModuleBaseName() override final {return "IFileSystem";};
    virtual const char* GetModuleName() override {return "IFileSystem";};

    virtual void SetBasePath(const std::string & strBasePath, const std::string & strPath) = 0;
    virtual void ClearBasePaths() = 0;
    virtual std::string ReadFile(const std::string &strFilePath) = 0;
};