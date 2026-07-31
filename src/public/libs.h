#pragma once

#include "string"
#include "mytypes.h"


class IMyLib
{
public:
    virtual void bebra() = 0;
    virtual void aboba() = 0;
};

class IModuleBase
{
public:
    virtual ~IModuleBase() = default;

    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;

    // Get the name of the interface that this module implements.
    //
    // GetModuleBaseName should not be implemented/overrided by anything other than
    // general interface.
    //
    // For example SDL3InputManager should not override GetModuleBaseName 
    // that is already implemented by IInputManager.
    virtual const char* GetModuleBaseName() = 0;
    
    // Get the name of the class that has implemented module interface.
    virtual const char* GetModuleName() = 0;
};


IModuleBase* LIB_LoadModule(const std::string &path);

#define LIB_MODULE_EXPORT_FUNC_NAME "GetModuleObject"
typedef IModuleBase* (*GetModuleFunc_t)();
#define LIB_EXPOSE_MODULE_OBJECT(moduleobj) EXPORT IModuleBase* GetModuleObject() {return (IModuleBase*)&moduleobj;};