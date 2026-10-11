#include "basicfilesystem.h"
#include "cmd/cmd.h"
#include "loaders/imageloader.h"
#include "loaders/objloader.h"

#include <cstdio>
#include <cerrno>
#include <cstring>

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#endif

#ifndef FS_VERBOSE_LOG
#define FS_VERBOSE_LOG 0
#endif

#if FS_VERBOSE_LOG
#define FS_VERBOSE(...) CMD::Msg(__VA_ARGS__)
#else
#define FS_VERBOSE(...)
#endif

#define FS_ERROR(...) CMD::Msg(__VA_ARGS__)

#define FS_ROOT_NAME "root"

// just because im lazy
namespace fs = std::filesystem;

struct FileHandleImpl
{
    FILE* fp = nullptr;
    fs::path path;
};

const modeldata_t CBasicFileSystem::s_EmptyModel;
const texturedata_t CBasicFileSystem::s_EmptyTexture;

static fs::path GetExecutableDir()
{
    std::error_code ec;

#ifdef PLATFORM_WINDOWS
    char buff[MAX_PATH];
    DWORD len = GetModuleFileNameA(nullptr, buff, MAX_PATH);
    fs::path exe = len ? fs::path(std::string(buff, len)) : fs::path();
#else
    fs::path exe = fs::read_symlink("/proc/self/exe", ec);
#endif

    if(ec || exe.empty())
    {
        FS_ERROR("FS: Unable to get executable path, using current directory.\n");
        return fs::current_path();
    }

    return exe.parent_path();
}

CBasicFileSystem::CBasicFileSystem()
{
    m_ExeDir = GetExecutableDir();
    ClearBasePaths();
}

CBasicFileSystem::~CBasicFileSystem()
{
    CBasicFileSystem::Shutdown();
}

bool CBasicFileSystem::Initialize()
{
    return true;
}

void CBasicFileSystem::Shutdown()
{
    FlushCache();
}

std::string CBasicFileSystem::NormalizeBaseName(const std::string &strBasePath)
{
    if(strBasePath.empty())
        return FS_ROOT_NAME;

    return ToLowerRet(strBasePath);
}

const std::vector<std::string>* CBasicFileSystem::FindBasePath(const std::string &strBasePath) const
{
    auto it = m_mapBasePaths.find(NormalizeBaseName(strBasePath));
    if(it == m_mapBasePaths.end() || it->second.empty())
    {
        FS_ERROR("FS: Unknown base path \"%s\".\n", strBasePath.c_str());
        return nullptr;
    }

    return &(it->second);
}

bool CBasicFileSystem::InsertBasePath(const std::string &strBasePath, const std::string &strFolder, bool bFront)
{
    if(strFolder.empty())
    {
        FS_ERROR("FS: Folder for base path \"%s\" should not be empty.\n", strBasePath.c_str());
        return false;
    }

    std::vector<std::string> &vecFolders = m_mapBasePaths[NormalizeBaseName(strBasePath)];
    std::string strNew = FixSlashesStdRet(strFolder);

    if(bFront)
        vecFolders.insert(vecFolders.begin(), strNew);
    else
        vecFolders.push_back(strNew);

    return true;
}

bool CBasicFileSystem::AddBasePathBack(const std::string &strBasePath, const std::string &strFolder)
{
    return InsertBasePath(strBasePath, strFolder, false);
}

bool CBasicFileSystem::AddBasePathFront(const std::string &strBasePath, const std::string &strFolder)
{
    return InsertBasePath(strBasePath, strFolder, true);
}

bool CBasicFileSystem::ReplaceBasePath(const std::string &strBasePath, uint iIndex, const std::string &strFolder)
{
    if(strFolder.empty())
    {
        FS_ERROR("FS: Folder for base path \"%s\" should not be empty.\n", strBasePath.c_str());
        return false;
    }

    auto it = m_mapBasePaths.find(NormalizeBaseName(strBasePath));
    if(it == m_mapBasePaths.end() || iIndex >= it->second.size())
    {
        FS_ERROR("FS: Unable to replace folder %u of base path \"%s\".\n", iIndex, strBasePath.c_str());
        return false;
    }

    it->second[iIndex] = FixSlashesStdRet(strFolder);
    return true;
}

void CBasicFileSystem::ClearBasePaths()
{
    m_mapBasePaths.clear();

    // should always be there
    m_mapBasePaths[FS_ROOT_NAME].push_back("." + std::string(1, CORRECT_PATH_SEPARATOR));
}

bool CBasicFileSystem::NormalizeRelativePath(const std::string &strPath, std::string &strOut)
{
    if(strPath.empty() || IsAbsolutePath(strPath))
    {
        FS_ERROR("FS: Path \"%s\" should not be empty or absolute.\n", strPath.c_str());
        return false;
    }

    fs::path normalized = fs::path(FixSlashesStdRet(strPath, '/')).lexically_normal();

    if(normalized.empty() || normalized.has_root_path() || *normalized.begin() == "..")
    {
        FS_ERROR("FS: Path \"%s\" is not inside of the base path.\n", strPath.c_str());
        return false;
    }

    // empty file name.
    if(!normalized.has_filename())
    {
        FS_ERROR("FS: Path \"%s\" should contain a file name.\n", strPath.c_str());
        return false;
    }

    strOut = normalized.generic_string();
    return true;
}

bool CBasicFileSystem::FindFile(const std::string &strBasePath, const std::string &strPath, fs::path &outFull, std::string &strKey) const
{
    std::string strRel;
    if(!NormalizeRelativePath(strPath, strRel))
        return false;

    const std::vector<std::string>* pFolders = FindBasePath(strBasePath);
    if(!pFolders)
        return false;

    strKey = ToLowerRet(strRel);

    std::error_code ec;
    for(const std::string &strFolder : *pFolders)
    {
        fs::path folder = m_ExeDir / strFolder;

        fs::path asIs = folder / strRel;
        FS_VERBOSE("FS: Looking for \"%s\".\n", asIs.string().c_str());
        if(fs::is_regular_file(asIs, ec))
        {
            outFull = asIs;
            return true;
        }

#ifdef PLATFORM_POSIX
        // on posix we do additional try for lowercase path.
        if(strKey != strRel)
        {
            fs::path lowered = folder / strKey;
            FS_VERBOSE("FS: Looking for \"%s\".\n", lowered.string().c_str());
            if(fs::is_regular_file(lowered, ec))
            {
                outFull = lowered;
                return true;
            }
        }
#endif
    }

    return false;
}

bool CBasicFileSystem::BuildWritePath(const std::string &strBasePath, const std::string &strPath, fs::path &outFull) const
{
    std::string strRel;
    if(!NormalizeRelativePath(strPath, strRel))
        return false;

    const std::vector<std::string>* pFolders = FindBasePath(strBasePath);
    if(!pFolders)
        return false;

    outFull = m_ExeDir / pFolders->front() / ToLowerRet(strRel);
    return true;
}

bool CBasicFileSystem::MakeCacheKey(const std::string &strPath, std::string &strKey)
{
    if(!NormalizeRelativePath(strPath, strKey))
        return false;

    ToLower(strKey);
    return true;
}

// lowlevel bullshit began

std::string CBasicFileSystem::BuildModeString(FileMode_t mode, bool bWrite, bool bAllowReadWrite)
{
    std::string strMode;

    if(bWrite)
        strMode = (mode & FS_APPEND) ? "a+" : "w+";
    else
        strMode = bAllowReadWrite ? "r+" : "r";

    if(mode & FS_BINARY)
        strMode += 'b';

    return strMode;
}

FileHandle_t CBasicFileSystem::OpenInternal(const fs::path &fullPath, const std::string &strMode)
{
    FILE* fp = fopen(fullPath.string().c_str(), strMode.c_str());
    if(!fp)
        return FILESYSTEM_INVALID_HANDLE;

    FileHandleImpl* pHandle = new FileHandleImpl();
    pHandle->fp = fp;
    pHandle->path = fullPath;
    return pHandle;
}

bool CBasicFileSystem::FileExists(const std::string &strBasePath, const std::string &strPath)
{
    fs::path full;
    std::string strKey;
    return FindFile(strBasePath, strPath, full, strKey);
}

FileHandle_t CBasicFileSystem::OpenFileRead(const std::string &strBasePath, const std::string &strPath, FileMode_t mode)
{
    fs::path full;
    std::string strKey;
    if(!FindFile(strBasePath, strPath, full, strKey))
    {
        FS_ERROR("FS: Could not find \"%s\" in \"%s\".\n", strPath.c_str(), strBasePath.c_str());
        return FILESYSTEM_INVALID_HANDLE;
    }

    FS_VERBOSE("FS: Open for read: %s\n", full.string().c_str());

    // might be a read only file.
    FileHandle_t hFile = OpenInternal(full, BuildModeString(mode, false, true));
    if(!hFile)
        hFile = OpenInternal(full, BuildModeString(mode, false, false));

    if(!hFile)
        FS_ERROR("FS: Could not open \"%s\": %s.\n", full.string().c_str(), std::strerror(errno));

    return hFile;
}

FileHandle_t CBasicFileSystem::OpenFileWrite(const std::string &strBasePath, const std::string &strPath, FileMode_t mode)
{
    fs::path full;
    if(!BuildWritePath(strBasePath, strPath, full))
        return FILESYSTEM_INVALID_HANDLE;

    FS_VERBOSE("FS: Open for write: %s\n", full.string().c_str());

    std::error_code ec;
    fs::create_directories(full.parent_path(), ec);
    if(ec)
    {
        FS_ERROR("FS: Failed to create directories \"%s\": %s.\n", full.parent_path().string().c_str(), ec.message().c_str());
        return FILESYSTEM_INVALID_HANDLE;
    }

    FileHandle_t hFile = OpenInternal(full, BuildModeString(mode, true, true));
    if(!hFile)
        FS_ERROR("FS: Could not open \"%s\": %s.\n", full.string().c_str(), std::strerror(errno));

    return hFile;
}

size_t CBasicFileSystem::ReadBytes(FileHandle_t hFile, void* pBuffer, size_t iSize)
{
    if(!hFile || !pBuffer)
        return 0;

    return fread(pBuffer, 1, iSize, hFile->fp);
}

size_t CBasicFileSystem::WriteBytes(FileHandle_t hFile, const void* pBuffer, size_t iSize)
{
    if(!hFile || !pBuffer)
        return 0;

    return fwrite(pBuffer, 1, iSize, hFile->fp);
}

size_t CBasicFileSystem::GetFileSize(FileHandle_t hFile)
{
    if(!hFile)
        return 0;

    fflush(hFile->fp);

    std::error_code ec;
    uintmax_t iSize = fs::file_size(hFile->path, ec);
    return ec ? 0 : static_cast<size_t>(iSize);
}

void CBasicFileSystem::CloseFile(FileHandle_t hFile)
{
    if(!hFile)
        return;

    fclose(hFile->fp);
    delete hFile;
}

// ---------------------------------------------------------------------------
// whole files
// ---------------------------------------------------------------------------

bool CBasicFileSystem::ReadWhole(const std::string &strBasePath, const std::string &strPath, FileMode_t mode, std::string &strOut)
{
    FileHandle_t hFile = OpenFileRead(strBasePath, strPath, mode);
    if(!hFile)
        return false;

    strOut.resize(GetFileSize(hFile));

    // In text mode on Windows we can get less bytes than file size.
    size_t iRead = ReadBytes(hFile, strOut.data(), strOut.size());
    strOut.resize(iRead);

    CloseFile(hFile);
    return true;
}

std::string CBasicFileSystem::ReadFileText(const std::string &strBasePath, const std::string &strPath)
{
    std::string strContent;
    ReadWhole(strBasePath, strPath, FS_NORMAL, strContent);
    return strContent;
}

std::vector<uint8> CBasicFileSystem::ReadFileBinary(const std::string &strBasePath, const std::string &strPath)
{
    std::string strContent;
    if(!ReadWhole(strBasePath, strPath, FS_BINARY, strContent))
        return {};

    const uint8* pData = reinterpret_cast<const uint8*>(strContent.data());
    return std::vector<uint8>(pData, pData + strContent.size());
}

bool CBasicFileSystem::WriteFileText(const std::string &strBasePath, const std::string &strPath, const std::string &strContent, FileMode_t mode)
{
    FileHandle_t hFile = OpenFileWrite(strBasePath, strPath, mode);
    if(!hFile)
        return false;

    size_t iWritten = WriteBytes(hFile, strContent.data(), strContent.size());
    CloseFile(hFile);

    if(iWritten != strContent.size())
    {
        FS_ERROR("FS: An error occurred while writing \"%s\": %s.\n", strPath.c_str(), std::strerror(errno));
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------------
// assets
// ---------------------------------------------------------------------------

bool CBasicFileSystem::PrecacheModel(const std::string &strBasePath, const std::string &strPath)
{
    std::string strKey;
    if(!MakeCacheKey(strPath, strKey))
        return false;

    if(m_mapModels.contains(strKey))
        return true;

    std::string strObj;
    if(!ReadWhole(strBasePath, strPath, FS_NORMAL, strObj))
        return false;

    modeldata_t model;
    std::string strError;
    if(!LoadObjFromMemory(strObj, model, strError))
    {
        FS_ERROR("FS: Failed to load model \"%s\": %s\n", strPath.c_str(), strError.c_str());
        return false;
    }

    m_mapModels[strKey] = std::move(model);
    return true;
}

bool CBasicFileSystem::PrecacheTexture(const std::string &strBasePath, const std::string &strPath)
{
    std::string strKey;
    if(!MakeCacheKey(strPath, strKey))
        return false;

    if(m_mapTextures.contains(strKey))
        return true;

    std::string strImage;
    if(!ReadWhole(strBasePath, strPath, FS_BINARY, strImage))
        return false;

    texturedata_t texture;
    if(!LoadImageFromMemory(reinterpret_cast<const uint8*>(strImage.data()), strImage.size(), texture))
    {
        FS_ERROR("FS: Failed to decode texture \"%s\".\n", strPath.c_str());
        return false;
    }

    m_mapTextures[strKey] = std::move(texture);
    return true;
}

const modeldata_t& CBasicFileSystem::GetModelData(const std::string &strBasePath, const std::string &strPath)
{
    if(!PrecacheModel(strBasePath, strPath))
        return s_EmptyModel;

    std::string strKey;
    MakeCacheKey(strPath, strKey);
    return m_mapModels[strKey];
}

const texturedata_t& CBasicFileSystem::GetTextureData(const std::string &strBasePath, const std::string &strPath)
{
    if(!PrecacheTexture(strBasePath, strPath))
        return s_EmptyTexture;

    std::string strKey;
    MakeCacheKey(strPath, strKey);
    return m_mapTextures[strKey];
}

void CBasicFileSystem::FlushCache()
{
    m_mapModels.clear();
    m_mapTextures.clear();
}

static CBasicFileSystem g_BasicFileSystem;

extern "C"
{
LIB_EXPOSE_MODULE_OBJECT(g_BasicFileSystem);
}
