#include "shared.h"
#include "ECS.h"

void ProcessEntities()
{
    for( auto EntityPair : g_ExistingEntities )
    {
        IEntity* pEntity = EntityPair.second;
        if(pEntity->IsMarkedForDelete())
        {
            g_ExistingEntities.erase(EntityPair.first);
            delete pEntity;
            continue;
        }

        if(pEntity->IsEnabled())
        {
            pEntity->TickThink();
        }
    }
}