#pragma once
#include "stdio.h"
#include "string"
#include <cstdarg>

inline void ConMsg(std::string str, ...)
{
    char formattedMessage[2048];
    va_list args; 
	va_start( args, str ); 
    vsnprintf(formattedMessage, 2048, str.c_str(), args);
	va_end( args );

    fputs(formattedMessage, stdout);
}