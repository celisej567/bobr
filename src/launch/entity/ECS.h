#pragma once

#include "string"
#include "vector"
#include "map"
#include "unordered_map"
#include "functional"
#include "glm.hpp"
#include "mytypes.h"

#include "iostream"

class IEntity;

typedef std::unordered_map< std::string, std::function<IEntity*()>> ClassnameMap_t;
typedef std::map<uint, IEntity*> ExistingEntityMap_t;

template <class T>
class CEntityClassnameBuilder;

inline IEntity* CreateEntity(std::string classname);
inline void SpawnEntity(IEntity* pEntity);
inline void DeleteEntity(IEntity* pEntity);
void ProcessEntitiesTick();
void ProcessEntitiesFrame();

// TODO (celisej): make some kind of InitializeEntitySystem() function or something
//                 that will handle g_EntityClassnameMap creation,
//                 classname-constructor link, etc... and call it seperatly in main()

// gives you a map of existing entities. 
// "Existing entites" includes created entities but not properly spawned yet. 
inline ExistingEntityMap_t& ExistingEntities()
{
    static ExistingEntityMap_t g_ExistingEntities;
    return g_ExistingEntities;
}

inline ClassnameMap_t& EntityClassnameMap()
{
    static ClassnameMap_t g_EntityClassnameMap;
    return g_EntityClassnameMap;
}

#define ENTITY_INVALID_INDEX 0xFFFFFFFF

class IEntity
{
public:

    virtual void Spawn() = 0;
    virtual void Tick() = 0;
    virtual void Think() = 0;

    virtual void Frame() = 0;

    virtual bool IsEnabled() = 0;
    virtual bool IsMarkedForDelete() = 0;

    virtual bool IsVisible() = 0;
    virtual void SetVisible(bool bEnable) = 0;

    virtual void Enable() = 0;
    virtual void Disable() = 0;
    virtual void MarkForDelete() = 0;

    virtual glm::vec3 GetAbsPos() = 0;
    virtual glm::vec3 GetAbsRot() = 0;

    virtual void SetAbsPos(glm::vec3 vec) = 0;
    virtual void SetAbsRot(glm::vec3 vec) = 0;

    virtual uint GetEntityIndex() = 0;
    virtual std::string GetClassname() = 0;

    virtual void ForceDelete() = 0;

protected:

    virtual void SetEntityIndex(uint index) = 0;
    virtual void SetClassname(std::string classname) = 0;

public:
    friend IEntity* CreateEntity(std::string classname);
    friend void SpawnEntity(IEntity* pEntity);
    friend void DeleteEntity(IEntity* pEntity);
};

template <class T>
class CEntityClassnameBuilder
{
public:

    CEntityClassnameBuilder(std::string classname)
    {
        //TODO (celisej): rn its called mid initialization of the whole program. This is bad.
        //                  Make it so it will call some dedicated function in main to do that.
        EntityClassnameMap()[classname] = []() -> IEntity* { return new T(); };
    }
};

#define LINK_CLASSNAME_TO_ENTITY(mapClassName,DLLClassName) \
	static CEntityClassnameBuilder<DLLClassName> mapClassName( #mapClassName )


// Creates Entity based on classname but does not initiate Spawn().
// If unknown classname is given returns NULL.
//
// TODO (celisej): Make it so entities can be created using costructor
//                  and will be properly handled that way as well
//                  without calling this function
inline IEntity* CreateEntity(std::string classname)
{
    if(!EntityClassnameMap().contains(classname))
    {
        std::cout << "Unable to spawn entity classname: " << classname << std::endl;
        return NULL;
    }

    uint iLastEntIndex = ENTITY_INVALID_INDEX;
    if( !(ExistingEntities().empty()) )
        iLastEntIndex = (std::prev(ExistingEntities().end()))->first;

    uint iEntIndex = 0;
    if(iLastEntIndex != ENTITY_INVALID_INDEX)
        iEntIndex = iLastEntIndex+1;

    IEntity* pEntity = EntityClassnameMap()[classname]();
    pEntity->SetClassname(classname);
    pEntity->SetEntityIndex(iEntIndex);

    ExistingEntities()[iEntIndex] = pEntity;

    return pEntity;
}

// Initiates propper Spawn() on already existing entity.
inline void SpawnEntity(IEntity* pEntity)
{
    if(!pEntity || pEntity->GetEntityIndex() == ENTITY_INVALID_INDEX)
        std::abort();

    pEntity->Enable();
    pEntity->Spawn();
    
}

inline void DeleteEntity(IEntity* pEntity)
{
    pEntity->Disable();
    pEntity->SetVisible(false);
    pEntity->MarkForDelete();
}
