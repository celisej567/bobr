#pragma once

#include "string"
#include <cstdarg>
#include <cstring>

#if defined(__gnu_linux__) || defined(BSD)
    #define PLATFORM_POSIX 1

    #define MAX_PATH PATH_MAX

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


// move this somewhere else

#ifdef PLATFORM_WINDOWS
#define CORRECT_PATH_SEPARATOR '\\'
#define INCORRECT_PATH_SEPARATOR '/'
#elif PLATFORM_POSIX
#define CORRECT_PATH_SEPARATOR '/'
#define INCORRECT_PATH_SEPARATOR '\\'
#endif

inline bool IsAbsolutePath( const std::string pStr )
{
	return ( pStr[0] && pStr[1] == ':' ) || pStr[0] == '/' || pStr[0] == '\\';
}

inline void FixSlashes( char *pname, char separator = CORRECT_PATH_SEPARATOR )
{
	while ( *pname )
	{
		if ( *pname == INCORRECT_PATH_SEPARATOR || *pname == CORRECT_PATH_SEPARATOR )
		{
			*pname = separator;
		}
		pname++;
	}
}

inline void FixSlashesStd( std::string &pname, char separator = CORRECT_PATH_SEPARATOR )
{
    for (char& ch : pname)
    {
        if (ch == INCORRECT_PATH_SEPARATOR || ch == CORRECT_PATH_SEPARATOR)
        {
            ch = separator;
        }
    }
}

inline std::string FixSlashesRet(char * pname, char separator = CORRECT_PATH_SEPARATOR)
{
    std::string result = pname;
    FixSlashesStd(result, separator);
    return result;
}

inline std::string FixSlashesStdRet(const std::string& pname, char separator = CORRECT_PATH_SEPARATOR)
{
    std::string result = pname;
    FixSlashesStd(result, separator);
    return result;
}

inline std::string Vars(const char* str, ...)
{
    char formattedMessage[4096];
    va_list args; 
	va_start( args, str );
    vsnprintf(formattedMessage, 4096, str, args);
	va_end( args );

    return std::string(formattedMessage);
}

inline void ToLower(std::string& str)
{
    for (char &c : str) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    };
}

inline std::string ToLowerRet(const std::string& str)
{
    std::string newstr = str;
    for (char &c : newstr) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    };

    return newstr;
}

inline void ToLower(char* str)
{
    uint len = strlen(str);
    for (uint i = 0; i < len; i++)
    {
        str[i] = std::tolower(str[i]);
    };
}

inline const char* ToLowerRet(const char* str)
{
    uint len = strlen(str);
    char* formattedMessage = new char[len];
    for (uint i = 0; i < len; i++)
    {
        formattedMessage[i] = std::tolower(str[i]);
    };

    return formattedMessage;
}