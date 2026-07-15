#include "iostream"
#include "CRotatingModel.h"
#include "glm.hpp"

LINK_CLASSNAME_TO_ENTITY( rotating_model, CRotatingModel );

void CRotatingModel::Think()
{
    BaseClass::Think();
    std::cout << GetEntityIndex() << ": CRotatingModel THINK!!!" << std::endl;
    SetAbsRot( GetAbsRot() + glm::vec3(10,0,0) );
    m_flNextThink = curtime + 1.0f;
}