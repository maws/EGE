#include "EntityManager.h"

#include <assert.h>

namespace EGE
{
	EntityManager::EntityManager()
	{
		m_entityGeneration.reserve(64);
		m_freeEntities.reserve(64);
	}

	Entity EntityManager::createEntity()
	{
		unsigned index;
		if (m_freeEntities.size() > MIN_NUM_FREE_ENTITIES)
		{
			index = m_freeEntities.front();
			m_freeEntities.erase(m_freeEntities.begin());
		}
		else
		{
			m_entityGeneration.push_back(0);
			index = m_entityGeneration.size() - 1;
			assert(index < (1 << ENTITY_INDEX_BITS));
		}

		unsigned id = 0;
		id |= index & ENTITY_INDEX_MASK;
		id |= (m_entityGeneration[index] & ENTITY_GENERATION_MASK) << ENTITY_INDEX_BITS;

		Entity entity(id);
		return entity;
	}

	void EntityManager::removeEntity(Entity entity)
	{
		unsigned index = entity.getIndex();
		++m_entityGeneration[index];
		m_freeEntities.push_back(index);
	}

	bool EntityManager::alive(Entity entity) const
	{
		return m_entityGeneration[entity.getIndex()] == entity.getGeneration();
	}
}