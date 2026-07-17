#pragma once

#include "string"
#include "mytypes.h"


class IMyLib
{
public:
    virtual void bebra() = 0;
    virtual void aboba() = 0;
};


#define INTERFACE_EXPOSE_OBJECT(interf,obj) interf * Get##interf##Interface() { return obj; }

//for now will use this.
#define INTERFACE_GET_FUNC_NAME(interf) "Get"#interf"Interface"