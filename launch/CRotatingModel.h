#pragma once

#include "shared.h"
#include "CModelEntity.h"

class CRotatingModel : public CModelEntity
{
    BASECLASS(CRotatingModel, CModelEntity);
public:
    virtual void Think();
};