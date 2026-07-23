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
    //virtual const char* GetModuleName() = 0;
};


IModuleBase* LIB_LoadModule(const std::string &path);

#define LIB_MODULE_EXPORT_FUNC_NAME "GetModuleObject"
typedef IModuleBase* (*GetModuleFunc_t)();
#define LIB_EXPOSE_MODULE_OBJECT(moduleobj) IModuleBase* GetModuleObject() {return (IModuleBase*)&moduleobj;};