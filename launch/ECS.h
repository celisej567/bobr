#pragma once

#include "string"
#include "vector"
#include "map"
#include "unordered_map"
#include "functional"

class IEntity;

template <class T>
class CEntityClassnameBuilder;

inline IEntity* CreateEntity(std::string classname);
inline void SpawnEntity(IEntity* pEntity);
inline void DeleteEntity(IEntity* pEntity);
void ProcessEntities();

inline std::map<uint, IEntity*> g_ExistingEntities;
inline std::unordered_map< std::string, std::function<IEntity*()>> g_EntityClassnameMap;

#define ENTITY_INVALID_INDEX 0xFFFFFFFF

class IEntity
{
public:
    virtual void Spawn() = 0;
    virtual void TickThink() = 0;
    virtual void Think() = 0;

    virtual bool IsEnabled() { return m_bEnabled; };
    virtual bool IsMarkedForDelete() { return m_bMarkForDelete; };

    virtual void Enable() {m_bEnabled=true;};
    virtual void Disable() {m_bEnabled=false;};

    virtual uint GetEntityIndex() {return m_uEntIndex;};
    virtual std::string GetClassname() {return m_strClassname;};

protected:

    uint m_uEntIndex = ENTITY_INVALID_INDEX;
    std::string m_strClassname;

    bool m_bEnabled = false;
    bool m_bMarkForDelete = false;

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
        g_EntityClassnameMap[classname] = []() -> IEntity* { return new T(); };
    }
};

#define LINK_CLASSNAME_TO_ENTITY(mapClassName,DLLClassName) \
	static CEntityClassnameBuilder<DLLClassName> mapClassName( #mapClassName )

inline IEntity* CreateEntity(std::string classname)
{
    if(!g_EntityClassnameMap.contains(classname))
        return NULL;

    uint iLastEntIndex = ENTITY_INVALID_INDEX;
    if( !(g_ExistingEntities.empty()) )
        uint iLastEntIndex = (std::prev(g_ExistingEntities.end()))->first;

    uint iEntIndex = 0;
    if(iLastEntIndex != ENTITY_INVALID_INDEX)
        iEntIndex = iLastEntIndex+1;

    IEntity* pEntity = g_EntityClassnameMap[classname]();
    pEntity->m_strClassname = classname;
    pEntity->m_uEntIndex = iEntIndex;
    g_ExistingEntities[iEntIndex] = pEntity;

    return pEntity;
}


inline void SpawnEntity(IEntity* pEntity)
{
    if(pEntity->m_uEntIndex == ENTITY_INVALID_INDEX)
        std::abort();

    pEntity->Enable();
    pEntity->Spawn();
    
}

inline void DeleteEntity(IEntity* pEntity)
{
    pEntity->Disable();
    pEntity->m_bMarkForDelete = true;
}
