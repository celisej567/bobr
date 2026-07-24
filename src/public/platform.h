#pragma once

#if defined(__gnu_linux__) || defined(BSD)
    #define PLATFORM_POSIX 1

    #ifdef __gnu_linux__
        #define PLATFORM_LINUX 1
        #define LINUX 1
    #elifdef _SYSTYPE_BSD
        #define PLATFORM_BSD 1
    #endif

#elifdef WIN32
    #define PLATFORM_WINDOWS 1
#else
#error "Platform not implemented."
#endif