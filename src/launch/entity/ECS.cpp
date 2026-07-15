#include "shared.h"
#include "ECS.h"

void ProcessEntitiesTick()
{
    auto& entities = ExistingEntities();
    for (auto it = entities.begin(); it != entities.end(); )
    {
        IEntity* pEntity = it->second;
        if(pEntity->IsMarkedForDelete())
        {
            pEntity->ForceDelete();
            it = entities.erase(it);
            continue;
        }

        if(pEntity->IsEnabled())
        {
            pEntity->Tick();
        }
        ++it;
    }
}

void ProcessEntitiesFrame()
{
    for( auto EntityPair : ExistingEntities() )
    {
        IEntity* pEntity = EntityPair.second;
        if(pEntity->IsMarkedForDelete())
        {
            ExistingEntities().erase(EntityPair.first);
            pEntity->ForceDelete();
            continue;
        }

        if( !(pEntity->IsVisible()) )
            continue;
        

        if(pEntity->IsEnabled())
        {
            pEntity->Frame();
        }
    }
}