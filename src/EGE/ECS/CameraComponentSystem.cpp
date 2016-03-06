#include <EGE\ECS\CameraComponentSystem.h>

#include <EGE\Memory\Allocator.h>
#include <EGE\ECS\EntityWorld.h>
#include <EGE\ECS\TransformComponentSystem.h>

#include <assert.h>

using namespace vmath;

namespace EGE
{
	CameraComponentSystem::CameraComponentSystem(Allocator* allocator, EntityWorld* world)
	{
		m_allocator = allocator;
		m_transformSystem = world->m_transformComponentSystem;
		memset(&m_data, 0, sizeof(m_data));

		allocate(4);
	}

	CameraComponentSystem::~CameraComponentSystem()
	{
		m_allocator->deallocate(m_data.buffer);
	}

	CameraComponentSystem::Component CameraComponentSystem::addComponent(Entity e)
	{
		if (m_data.numOccupied >= m_data.numAllocated)
		{
			allocate(m_data.numAllocated * 2);
		}

		Component ins;
		ins.m_id = m_data.numOccupied++;
		
		m_data.entity[ins.m_id] = e;

		m_entityComponentMap[e.m_id] = ins.m_id;

		return ins;
	}

	CameraComponentSystem::Component CameraComponentSystem::getComponent(Entity e)
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

	void CameraComponentSystem::removeComponent(Component ins)
	{
		unsigned indexOfLast = m_data.numOccupied - 1;
		Entity entityToRemove = m_data.entity[ins.m_id];
		Entity lastEntity = m_data.entity[indexOfLast];

		// Unless we are the last Component overwrite us with the last Component 
		if (!(ins.m_id == indexOfLast))
		{
			m_data.entity[ins.m_id] = m_data.entity[indexOfLast];
			m_data.projectionMatrix[ins.m_id] = m_data.projectionMatrix[indexOfLast];
			m_data.viewMatrix[ins.m_id] = m_data.viewMatrix[indexOfLast];
			m_entityComponentMap[lastEntity.m_id] = ins.m_id;
		}

		m_entityComponentMap.erase(entityToRemove.m_id);
		--m_data.numOccupied;
	}

	bool CameraComponentSystem::isValid(Component ins)
	{
		if (ins.m_id != -1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void CameraComponentSystem::removeDeadEntities(const EntityManager& em)
	{
		unsigned aliveConsecutive = 0;
		while (m_data.numOccupied > 0 && aliveConsecutive < 4)
		{
			unsigned index = rand() % (m_data.numOccupied);
			if (em.alive(m_data.entity[index]))
			{
				++aliveConsecutive;
				continue;
			}
			Component ins;
			ins.m_id = index;
			removeComponent(ins);
		}
	}

	void CameraComponentSystem::tick()
	{
		for (unsigned i = 0; i < m_data.numOccupied; ++i)
		{
			Entity e = m_data.entity[i];
			auto transform = m_transformSystem->getComponent(e);
			if (m_transformSystem->isValid(transform))
			{
				vec3 rot = m_transformSystem->getWorldRotation(transform);
				mat4 transformMatrix = translate(m_transformSystem->getWorldPosition(transform)) * rotate(rot[0], rot[1], rot[2]);
				
				vec3 at = vec3(transformMatrix[2][0], transformMatrix[2][1], transformMatrix[2][2]);
				vec3 up = vec3(transformMatrix[1][0], transformMatrix[1][1], transformMatrix[1][2]);
				vec3 pos = vec3(transformMatrix[3][0], transformMatrix[3][1], transformMatrix[3][2]);
				m_data.viewMatrix[i] = lookat(pos, pos + at, up);
			}
		}
	}

	void CameraComponentSystem::setProjectionMatrix(Component ins, const mat4& proj)
	{
		m_data.projectionMatrix[ins.m_id] = proj;
	}

	void CameraComponentSystem::setViewMatrix(Component ins, const mat4& view)
	{
		m_data.viewMatrix[ins.m_id] = view;
	}

	void CameraComponentSystem::allocate(size_t size)
	{
		assert(size > m_data.numOccupied);

		Data newData;
		const size_t bytes = size * (sizeof(Entity) + sizeof(vmath::mat4) + sizeof(vmath::mat4));

		newData.buffer = m_allocator->allocate(bytes);

		newData.numOccupied = m_data.numOccupied;
		newData.numAllocated = size;

		newData.entity = static_cast<Entity*>(newData.buffer);
		newData.projectionMatrix = reinterpret_cast<vmath::mat4*>(newData.entity + size);
		newData.viewMatrix = reinterpret_cast<vmath::mat4*>(newData.projectionMatrix + size);

		memcpy(newData.entity, m_data.entity, m_data.numAllocated * sizeof(Entity));
		memcpy(newData.projectionMatrix, m_data.projectionMatrix, m_data.numAllocated * sizeof(vmath::mat4));
		memcpy(newData.viewMatrix, m_data.viewMatrix, m_data.numAllocated * sizeof(vmath::mat4));

		m_allocator->deallocate(m_data.buffer);
		m_data = newData;
	}
}