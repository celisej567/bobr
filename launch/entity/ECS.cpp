#include "shared.h"
#include "ECS.h"

void ProcessEntitiesThink()
{
    for( auto EntityPair : ExistingEntities() )
    {
        IEntity* pEntity = EntityPair.second;
        if(pEntity->IsMarkedForDelete())
        {
            ExistingEntities().erase(EntityPair.first);
            delete pEntity;
            continue;
        }

        if(pEntity->IsEnabled())
        {
            pEntity->Tick();
        }
    }
}

void ProcessEntitiesFrame()
{
    for( auto EntityPair : ExistingEntities() )
    {
        IEntity* pEntity = EntityPair.second;
        if( !(pEntity->IsVisible()) )
            continue;
        
        if(pEntity->IsMarkedForDelete())
        {
            ExistingEntities().erase(EntityPair.first);
            delete pEntity;
            continue;
        }

        if(pEntity->IsEnabled())
        {
            pEntity->Frame();
        }
    }
}