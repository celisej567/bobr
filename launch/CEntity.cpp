#include "shared.h"
#include "CEntity.h"
#include "iostream"

LINK_CLASSNAME_TO_ENTITY(base_entity, CEntity);

void CEntity::Spawn()
{

}

void CEntity::TickThink()
{
    if( m_flNextThink <= curtime )
        Think();

}

void CEntity::Think()
{
    std::cout << "THINK!!!" << std::endl;
    m_flNextThink = curtime + 0.5f;
}