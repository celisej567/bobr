#include "cbase.h"

#include "libs.h"
#include "cmd/cmd.h"


#ifdef PLATFORM_POSIX
#include <dlfcn.h>
#elif PLATFORM_WINDOWS
#include <windows.h>
#endif


IModuleBase* LIB_LoadModule(const std::string &path)
{
#ifdef PLATFORM_POSIX
    void* mylibdll = 0;
    mylibdll = dlopen(path.c_str(), RTLD_LAZY);

    if(!mylibdll)
    {
        const char* err = dlerror();
        CMD::Msg("Error Module %s : %s\n", path.c_str(), err);
        return 0;
    }

    GetModuleFunc_t my_func = (GetModuleFunc_t)dlsym(mylibdll, LIB_MODULE_EXPORT_FUNC_NAME);
    const char* err = dlerror();
    if(err)
    {
        CMD::Msg("Error: %s\n", err);
        dlclose(mylibdll);
        return 0;
    }

    return my_func();

#elif PLATFORM_WINDOWS

    char cwd[512];
    GetCurrentDirectory(512, cwd);
    CMD::Msg("Current directory: %s\n", cwd);

    
    if (GetFileAttributes(path.c_str()) == INVALID_FILE_ATTRIBUTES) {
        CMD::Msg("File does not exist: %s\n", path.c_str());
    }
    else {
        CMD::Msg("File exists: %s\n", path.c_str());
    }

    HMODULE hLib = LoadLibrary(path.c_str());
    if (hLib == NULL) {
        DWORD err = GetLastError();
        CMD::Msg("Error Module On Open %s : %lu\n", path.c_str(), err);
       
        LPSTR msgBuf = nullptr;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            nullptr, err, 0, (LPSTR)&msgBuf, 0, nullptr);
        CMD::Msg("System message: %s\n", msgBuf);
        LocalFree(msgBuf);
        return 0;
    }

    GetModuleFunc_t myFunc = (GetModuleFunc_t)GetProcAddress(hLib, LIB_MODULE_EXPORT_FUNC_NAME);
    if (myFunc == NULL)
    {
        DWORD err = GetLastError();
        CMD::Msg("Error Module On Func %s : %lu\n", path.c_str(), err);
        FreeLibrary(hLib);
        return 0;
    }


    return myFunc();
    
#endif
}