#pragma once
#include "vector"
#include "unordered_map"

class IEntity;

template <class T>
class CEntityClassnameBuilder;

IEntity* CreateEntity(std::string classname);

inline std::vector<IEntity*> g_ExistingEntities;
inline std::vector<IEntity*> g_SpawnedEntities;
inline std::unordered_map< std::string, void*> g_EntityClassnameMap;

#define ENTITY_INVALID_INDEX 0

class IEntity
{
public:

    virtual uint GetEntityIndex() {return entindex};

protected:

    uint m_uEntIndex = ENTITY_INVALID_INDEX;

public:
    friend IEntity* CreateEntity(std::string classname);
};

template <class T>
class CEntityClassnameBuilder
{
    EntityClassnameBuilder(std::string classname)
    {
        g_EntityClassnameMap[classname] = []() -> IEntity* { return new T(); };
    }
};

#define LINK_CLASSNAME_TO_ENTITY(mapClassName,DLLClassName) \
	static CEntityClassnameBuilder<DLLClassName> mapClassName( #mapClassName );

IEntity* CreateEntity(std::string classname)
{
    if(!g_EntityClassnameMap.contains(classname))
        return NULL;

    IEntity* pEntity = g_EntityClassnameMap[classname]();
    pEntity->m_uEntIndex = g_ExistingEntities.size() + 1;
    g_ExistingEntities.push_back(pEntity);

    return pEntity;
}
