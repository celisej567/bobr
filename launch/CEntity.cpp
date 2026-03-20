#include "shared.h"
#include "CEntity.h"
#include "iostream"

LINK_CLASSNAME_TO_ENTITY(base_entity, CEntity);

void CEntity::Spawn()
{

}

void CEntity::Tick()
{
    if( m_flNextThink <= curtime )
        Think();

}

void CEntity::Think()
{

}

void CEntity::Frame()
{

}