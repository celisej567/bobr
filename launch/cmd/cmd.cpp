#include "cmd.h"
#include <iostream>
#include <cstring>



char* CMD::m_Args[CMD_ARGS_RESERVE] = {};
uint CMD::m_ArgsCount = 0;

void CMD::ProcessArguments(int argc, char **argv)
{
    if(m_ArgsCount)
    {
        for(int i = 0; i < CMD_ARGS_RESERVE; i++)
        {
            // IMPORTANT
            // Intentially keeping old args and do not delete them because
            //   1: its longer to dealloc memory
            //   2: usually argument strings are handled by OS and can be shared between other parts of the program. 
            m_Args[i] = NULL;
        }
        m_ArgsCount = 0;
    }

    /// TODO (celisej): change this to new vector/stack/array type when one will be done
    if( argc > CMD_ARGS_MAX )
    {
        char buff[512];
        sprintf(buff, "Too many arguments passed. Got %d but MAX is %d.", argc, CMD_ARGS_MAX);
        std::cout << buff << std::endl;
        throw std::runtime_error(buff);
        std::exit(-1);
    }

    for(int i = 0; i<argc; i++)
    {
        m_Args[i] = argv[i];
    }

    m_ArgsCount = argc;
}

const char* CMD::GetArg(const char *strArg, const char *strDefault)
{
    uint argID = FindArg(strArg);
    if( !argID || argID+1 > m_ArgsCount || !(m_Args[argID+1]) )
        return strDefault;

    // the next one might be another argument.
    if( m_Args[argID+1][0] == '-' )
        return strDefault;

    return m_Args[argID+1];
}

bool CMD::GetArgBool( const char* strArg, bool bDefault )
{
    uint argID = FindArg(strArg);
    if( !argID || argID+1 > m_ArgsCount || !(m_Args[argID+1]) )
        return bDefault;

    if( m_Args[argID+1][0] == '-' )
        return bDefault;

    if( m_Args[argID+1][0] == 't' )
        return true;

    if( m_Args[argID+1][0] == 'f' )
        return false;

    return atoi(m_Args[argID+1]) > 0;
}

int CMD::GetArgInt( const char* strArg, int iDefault )
{
    uint argID = FindArg(strArg);
    if( !argID || argID+1 > m_ArgsCount || !(m_Args[argID+1]) )
        return iDefault;

    return atoi(m_Args[argID+1]);
}

float CMD::GetArgFloat( const char* strArg, float fDefault )
{
    uint argID = FindArg(strArg);
    if( !argID || argID+1 > m_ArgsCount || !(m_Args[argID+1]) )
        return fDefault;

    return atof(m_Args[argID+1]);
}

double CMD::GetArgDouble( const char* strArg, double dDefault )
{
    uint argID = FindArg(strArg);
    if( !argID || argID+1 > m_ArgsCount || !(m_Args[argID+1]) )
        return dDefault;

    return atof(m_Args[argID+1]);
}

uint CMD::FindArg( const char* strArg )
{
    for(uint i = 1; i < m_ArgsCount; i++)
    {
        if(strcmp(strArg, m_Args[i]) == 0)
        {
            return i;
        }
    }

    return 0;
}

const char* CMD::GetExecutable()
{
    return m_Args[0];
}