#include <EGE\ECS\MeshResourceComponentSystem.h>

#include <EGE\MeshResource.hpp>
#include <EGE\Memory\Allocator.h>
#include <EGE\ECS\EntityWorld.h>

#include <assert.h>

namespace EGE
{
	MeshResourceComponentSystem::MeshResourceComponentSystem(Allocator* allocator, EntityWorld* world)
	{
		m_allocator = allocator;
		m_data.numAllocated = 0;
		m_data.numOccupied = 0;
		m_data.buffer = nullptr;
		m_data.entity = nullptr;
		m_data.meshResource = nullptr;

		allocate(32);
	}

	MeshResourceComponentSystem::~MeshResourceComponentSystem()
	{
		m_allocator->deallocate(m_data.buffer);
	}

	void MeshResourceComponentSystem::allocate(size_t size)
	{
		assert(size > m_data.numOccupied);

		ComponentData newData;
		const size_t bytes = size * (
			sizeof(Entity) + 
			sizeof(uintptr_t));

		newData.buffer = m_allocator->allocate(bytes);

		newData.numOccupied = m_data.numOccupied;
		newData.numAllocated = size;

		newData.entity = static_cast<Entity*>(newData.buffer);
		newData.meshResource = reinterpret_cast<uintptr_t*>(newData.entity + size);

		memcpy(newData.entity, m_data.entity, m_data.numAllocated * sizeof(Entity));
		memcpy(newData.meshResource, m_data.meshResource, m_data.numAllocated * sizeof(uintptr_t));

		m_allocator->deallocate(m_data.buffer);
		m_data = newData;
	}

	void MeshResourceComponentSystem::removeComponent(Component ins)
	{
		unsigned indexOfLast = m_data.numOccupied - 1;
		Entity entityToRemove = m_data.entity[ins.m_id];
		Entity lastEntity = m_data.entity[indexOfLast];

		// Unless we are the last Component overwrite us with the last Component 
		if (!(ins.m_id == indexOfLast))
		{
			m_data.entity[ins.m_id] = m_data.entity[indexOfLast];
			m_data.meshResource[ins.m_id] = m_data.meshResource[indexOfLast];
			m_entityComponentMap[lastEntity.m_id] = ins.m_id;
		}

		m_entityComponentMap.erase(entityToRemove.m_id);
		--m_data.numOccupied;
	}

	MeshResourceComponentSystem::Component MeshResourceComponentSystem::addComponent(Entity e)
	{
		if (m_data.numOccupied >= m_data.numAllocated)
		{
			allocate(m_data.numAllocated * 2);
		}

		Component ins;
		ins.m_id = m_data.numOccupied++;

		m_data.entity[ins.m_id] = e;
		m_data.meshResource[ins.m_id] = 0;

		m_entityComponentMap[e.m_id] = ins.m_id;

		return ins;
	}

	MeshResourceComponentSystem::Component MeshResourceComponentSystem::getComponent(Entity e)
	{
		auto it = m_entityComponentMap.find(e.m_id);
		if (it != m_entityComponentMap.end())
		{
			Component ins;
			ins.m_id = it->second;
			return ins;
		}

		Component ins;
		ins.m_id = -1;
		return ins;
	}

	void MeshResourceComponentSystem::setMeshResource(Component ins, MeshResource* mesh)
	{
		m_data.meshResource[ins.m_id] = reinterpret_cast<uintptr_t>(mesh);
	}
}