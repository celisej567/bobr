#pragma once

#include "cbase.h"
#include "libs.h"
#include "filesystem/filesystem.h"
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

class CBasicFileSystem : public IFileSystem
{
public:
    CBasicFileSystem();
    virtual ~CBasicFileSystem();

    virtual bool Initialize() override;
    virtual void Shutdown() override;

    virtual const char* GetModuleName() override {return "CBasicFileSystem";};

    virtual bool AddBasePathBack(const std::string &strBasePath, const std::string &strFolder) override;
    virtual bool AddBasePathFront(const std::string &strBasePath, const std::string &strFolder) override;
    virtual bool ReplaceBasePath(const std::string &strBasePath, uint iIndex, const std::string &strFolder) override;
    virtual void ClearBasePaths() override;

    virtual bool FileExists(const std::string &strBasePath, const std::string &strPath) override;
    virtual FileHandle_t OpenFileRead(const std::string &strBasePath, const std::string &strPath, FileMode_t mode = FS_NORMAL) override;
    virtual FileHandle_t OpenFileWrite(const std::string &strBasePath, const std::string &strPath, FileMode_t mode = FS_NORMAL) override;
    virtual size_t ReadBytes(FileHandle_t hFile, void* pBuffer, size_t iSize) override;
    virtual size_t WriteBytes(FileHandle_t hFile, const void* pBuffer, size_t iSize) override;
    virtual size_t GetFileSize(FileHandle_t hFile) override;
    virtual void CloseFile(FileHandle_t hFile) override;

    virtual std::string ReadFileText(const std::string &strBasePath, const std::string &strPath) override;
    virtual std::vector<uint8> ReadFileBinary(const std::string &strBasePath, const std::string &strPath) override;
    virtual bool WriteFileText(const std::string &strBasePath, const std::string &strPath, const std::string &strContent, FileMode_t mode = FS_NORMAL) override;

    virtual bool PrecacheModel(const std::string &strBasePath, const std::string &strPath) override;
    virtual bool PrecacheTexture(const std::string &strBasePath, const std::string &strPath) override;
    virtual const modeldata_t& GetModelData(const std::string &strBasePath, const std::string &strPath) override;
    virtual const texturedata_t& GetTextureData(const std::string &strBasePath, const std::string &strPath) override;
    virtual void FlushCache() override;

private:
    // Lower cases the name, empty name becomes "root".
    static std::string NormalizeBaseName(const std::string &strBasePath);

    // Validates a user given path and returns it with correct slashes.
    // Fails on empty, absolute or escaping (..) paths.
    static bool NormalizeRelativePath(const std::string &strPath, std::string &strOut);

    static std::string BuildModeString(FileMode_t mode, bool bWrite, bool bAllowReadWrite);

    // Fills strFolders with the chain of a base path.
    const std::vector<std::string>* FindBasePath(const std::string &strBasePath) const;

    bool InsertBasePath(const std::string &strBasePath, const std::string &strFolder, bool bFront);

    // Finds an existing file going through the whole chain.
    // strKey is set to the relative path in lower case, same for every OS.
    bool FindFile(const std::string &strBasePath, const std::string &strPath, std::filesystem::path &outFull, std::string &strKey) const;

    // Builds a path to write to. Always the first folder in the chain and always lower case.
    bool BuildWritePath(const std::string &strBasePath, const std::string &strPath, std::filesystem::path &outFull) const;

    FileHandle_t OpenInternal(const std::filesystem::path &fullPath, const std::string &strMode);

    // Reads the whole file into strOut.
    bool ReadWhole(const std::string &strBasePath, const std::string &strPath, FileMode_t mode, std::string &strOut);

    // Only gets the cache key for a path.
    static bool MakeCacheKey(const std::string &strPath, std::string &strKey);

    std::filesystem::path m_ExeDir;
    std::unordered_map<std::string, std::vector<std::string>> m_mapBasePaths;

    // gonna be here for now
    std::unordered_map<std::string, modeldata_t> m_mapModels;
    std::unordered_map<std::string, texturedata_t> m_mapTextures;

    static const modeldata_t s_EmptyModel;
    static const texturedata_t s_EmptyTexture;
};
