#pragma once

#include "libs.h"
#include "filedata.h"
#include <string>
#include <vector>


struct FileHandleImpl;
typedef FileHandleImpl* FileHandle_t;

#define FILESYSTEM_INVALID_HANDLE nullptr


// Every file is opened for both reading and writing, flags only change how.
enum FileMode_t : unsigned int
{
    FS_NORMAL = 0,
    FS_APPEND = 1 << 0,
    FS_BINARY = 1 << 1,
};

inline FileMode_t operator|(FileMode_t a, FileMode_t b)
{
    return static_cast<FileMode_t>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}

// Every function that takes basePath and path requires both.
// Empty basePath means "root".
// "root" always exists and points to the launcher folder.
// basePath names are case insensitive.
// path must be relative and must not leave the base path folder.
class IFileSystem : public IModuleBase
{
public:

    virtual const char* GetModuleBaseName() override final {return "IFileSystem";};
    virtual const char* GetModuleName() override {return "IFileSystem";};

    // strBasePath is a key thast expands into a list of paths.
    // path with index 0 has the highest priority.

    virtual bool AddBasePathBack(const std::string &strBasePath, const std::string &strFolder) = 0;
    virtual bool AddBasePathFront(const std::string &strBasePath, const std::string &strFolder) = 0;
    virtual bool ReplaceBasePath(const std::string &strBasePath, uint iIndex, const std::string &strFolder) = 0;

    virtual void ClearBasePaths() = 0;

    virtual bool FileExists(const std::string &strBasePath, const std::string &strPath) = 0;

    virtual std::string ReadFileText(const std::string &strBasePath, const std::string &strPath) = 0;
    virtual std::vector<uint8> ReadFileBinary(const std::string &strBasePath, const std::string &strPath) = 0;
    virtual bool WriteFileText(const std::string &strBasePath, const std::string &strPath, const std::string &strContent, FileMode_t mode = FS_NORMAL) = 0;


    // Whatever was cached first wins until FlushCache.
    // Get* return an invalid empty object on failure.

    virtual bool PrecacheModel(const std::string &strBasePath, const std::string &strPath) = 0;
    virtual bool PrecacheTexture(const std::string &strBasePath, const std::string &strPath) = 0;

    virtual const modeldata_t& GetModelData(const std::string &strBasePath, const std::string &strPath) = 0;
    virtual const texturedata_t& GetTextureData(const std::string &strBasePath, const std::string &strPath) = 0;


    // lowlevel shit

    // Never creates or empties a file.
    virtual FileHandle_t OpenFileRead(const std::string &strBasePath, const std::string &strPath, FileMode_t mode = FS_NORMAL) = 0;

    // Always writes to the index 0 path.
    // Truncates the file unless FS_APPEND is set.
    virtual FileHandle_t OpenFileWrite(const std::string &strBasePath, const std::string &strPath, FileMode_t mode = FS_NORMAL) = 0;

    // REALLY lowlevel shit

    // Return amount of bytes actually read/written.
    virtual size_t ReadBytes(FileHandle_t hFile, void* pBuffer, size_t iSize) = 0;
    virtual size_t WriteBytes(FileHandle_t hFile, const void* pBuffer, size_t iSize) = 0;
    virtual size_t GetFileSize(FileHandle_t hFile) = 0;
    virtual void CloseFile(FileHandle_t hFile) = 0;

    // Invalidates every reference returned by Get*.
    virtual void FlushCache() = 0;
};
