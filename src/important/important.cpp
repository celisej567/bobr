#include "cbase.h"

#include "libs.h"
#include "cmd/cmd.h"


#ifdef PLATFORM_POSIX
#include <dlfcn.h>
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
#endif
}