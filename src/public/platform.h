#pragma once

#include "string"
#include <cstdarg>
#include <cstring>
#include <vector>
#include "mytypes.h"

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

inline std::string VarsStd(const std::string& str, ...)
{
    char formattedMessage[4096];
    va_list args;

    //there might be warning about variadic parameter
	va_start( args, (str.data()) );
    vsnprintf(formattedMessage, 4096, str.data(), args);
	va_end( args );

    return std::string(formattedMessage);
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

//TODO: std-less variant
inline std::vector<std::string> SplitStd(const std::string& str, char separator) 
{
    std::vector<std::string> strs;
    
    size_t start = 0;
    size_t end = str.find(separator);

    while (end != std::string::npos) {
        
        strs.push_back(str.substr(start, end - start));
        
        start = end + 1;
        end = str.find(separator, start);
    }
    
    strs.push_back(str.substr(start));

    return strs;
}


inline std::string RemoveLastStd(const std::string& str, char separator)
{
    size_t pos = str.rfind(separator);

    if (pos == std::string::npos) {
        return str;
    }

    return str.substr(0, pos);
}

inline char* RemoveLast(const char* str, char separator) 
{
    if (str == NULL) {
        return NULL;
    }

    const char* last_sep = strrchr(str, separator);

    size_t new_len;

    if (last_sep == NULL)
        new_len = strlen(str);
    else
        new_len = last_sep - str;

    char* new_str = (char*)malloc(new_len + 1);
    if (new_str == NULL) {
        return NULL;
    }

    strncpy(new_str, str, new_len);
    

    new_str[new_len] = '\0';

    return new_str;
}