#include <EGE\ECS\TransformComponentSystem.h>

#include <EGE\Memory\Allocator.h>
#include <EGE\ECS\EntityWorld.h>

#include <assert.h>

namespace EGE
{
	TransformComponentSystem::TransformComponentSystem(Allocator* allocator, EntityWorld* world)
	{
		m_allocator = allocator;
		memset(&m_data, 0, sizeof(m_data));
		allocate(32);
	}

	TransformComponentSystem::~TransformComponentSystem()
	{
		m_allocator->deallocate(m_data.buffer);
	}

	void TransformComponentSystem::allocate(size_t size)
	{
		assert(size > m_data.numOccupied);

		ComponentData newData;
		const size_t bytes = size * 
			(sizeof(Entity) + 
			sizeof(vmath::vec3) + 
			sizeof(vmath::vec3) +
			sizeof(vmath::vec3) +
			sizeof(vmath::vec3) +
			sizeof(Component) + 
			sizeof(Component) + 
			sizeof(Component) + 
			sizeof(Component));

		newData.buffer = m_allocator->allocate(bytes);

		newData.numOccupied = m_data.numOccupied;
		newData.numAllocated = size;

		newData.entity = static_cast<Entity*>(newData.buffer);
		newData.localPosition = reinterpret_cast<vmath::vec3*>(newData.entity + size);
		newData.localRotation = reinterpret_cast<vmath::vec3*>(newData.localPosition + size);
		newData.worldPosition = reinterpret_cast<vmath::vec3*>(newData.localRotation + size);
		newData.worldRotation = reinterpret_cast<vmath::vec3*>(newData.worldPosition + size);
		newData.parent = reinterpret_cast<Component*>(newData.worldRotation + size);
		newData.firstChild = reinterpret_cast<Component*>(newData.parent + size);
		newData.nextSibling = reinterpret_cast<Component*>(newData.firstChild + size);
		newData.prevSibling = reinterpret_cast<Component*>(newData.nextSibling + size);

		memcpy(newData.entity, m_data.entity, m_data.numAllocated * sizeof(Entity));
		memcpy(newData.localPosition, m_data.localPosition, m_data.numAllocated * sizeof(vmath::vec3));
		memcpy(newData.localRotation, m_data.localRotation, m_data.numAllocated * sizeof(vmath::vec3));
		memcpy(newData.worldPosition, m_data.worldPosition, m_data.numAllocated * sizeof(vmath::vec3));
		memcpy(newData.worldRotation, m_data.worldRotation, m_data.numAllocated * sizeof(vmath::vec3));
		memcpy(newData.parent, m_data.parent, m_data.numAllocated * sizeof(Component));
		memcpy(newData.firstChild, m_data.firstChild, m_data.numAllocated * sizeof(Component));
		memcpy(newData.nextSibling, m_data.nextSibling, m_data.numAllocated * sizeof(Component));
		memcpy(newData.prevSibling, m_data.prevSibling, m_data.numAllocated * sizeof(Component));

		m_allocator->deallocate(m_data.buffer);
		m_data = newData;
	}

	void TransformComponentSystem::destroyChildren(Component ins)
	{
		auto firstChild = m_data.firstChild[ins.id];
		if (isValid(firstChild))
		{
			auto lastChild = m_data.nextSibling[firstChild.id];
			while (isValid(lastChild))
			{
				auto nextChild = m_data.nextSibling[lastChild.id];
				if (isValid(nextChild))
				{
					lastChild = nextChild;
				}
				else
				{
					break;
				}
			}

			while (isValid(lastChild))
			{
				auto prevChild = m_data.prevSibling[lastChild.id];
				removeComponent(lastChild);
				lastChild = prevChild;
			}

		}
	}

	void TransformComponentSystem::removeComponent(Component ins)
	{
		destroyChildren(ins);

		unsigned indexOfLast = m_data.numOccupied - 1;
		Entity entityToRemove = m_data.entity[ins.id];
		Entity lastEntity = m_data.entity[indexOfLast];

		if (!(ins.id == indexOfLast))
		{
			m_data.entity[ins.id] = m_data.entity[indexOfLast];
			m_data.localPosition[ins.id] = m_data.localPosition[indexOfLast];
			m_data.localRotation[ins.id] = m_data.localRotation[indexOfLast];
			m_data.worldPosition[ins.id] = m_data.worldPosition[indexOfLast];
			m_data.worldRotation[ins.id] = m_data.worldRotation[indexOfLast];
			m_data.parent[ins.id] = m_data.parent[indexOfLast];
			m_data.firstChild[ins.id] = m_data.firstChild[indexOfLast];
			m_data.nextSibling[ins.id] = m_data.nextSibling[indexOfLast];
			m_data.prevSibling[ins.id] = m_data.prevSibling[indexOfLast];
			m_map[lastEntity.m_id] = ins.id;
		}

		m_map.erase(entityToRemove.m_id);
		--m_data.numOccupied;
	}

	void TransformComponentSystem::removeDeadEntities(const EntityManager& em)
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
			ins.id = index;
			removeComponent(ins);
		}
	}

	void TransformComponentSystem::setLocalPosition(Component ins, const vmath::vec3& v)
	{
		m_data.localPosition[ins.id] = v;

		Component parent = m_data.parent[ins.id];
		vmath::vec3 parentPosition;
		if (isValid(parent))
		{
			parentPosition = m_data.worldPosition[parent.id];
		}
		else
		{
			vmath::vec3 zero;
			zero[0] = 0;
			zero[1] = 0;
			zero[2] = 0;
			parentPosition = zero;
		}

		transformPosition(parentPosition, ins);
	}

	void TransformComponentSystem::setLocalRotation(Component ins, const vmath::vec3& r)
	{
		m_data.localRotation[ins.id] = r;

		Component parent = m_data.parent[ins.id];
		vmath::vec3 parentRotation;
		if (isValid(parent))
		{
			parentRotation = m_data.worldRotation[parent.id];
		}
		else
		{
			vmath::vec3 zero;
			zero[0] = 0;
			zero[1] = 0;
			zero[2] = 0;
			parentRotation = zero;
		}

		transformRotation(parentRotation, ins);
	}

	void TransformComponentSystem::transformPosition(const vmath::vec3& parent, Component ins)
	{
		m_data.worldPosition[ins.id] = m_data.localPosition[ins.id] + parent;

		Component child = m_data.firstChild[ins.id];
		while (isValid(child))
		{
			transformPosition(m_data.worldPosition[ins.id], child);
			child = m_data.nextSibling[child.id];
		}
	}

	void TransformComponentSystem::transformRotation(const vmath::vec3& parent, Component ins)
	{
		m_data.worldRotation[ins.id] = m_data.localRotation[ins.id] + parent;

		Component child = m_data.firstChild[ins.id];
		while (isValid(child))
		{
			transformRotation(m_data.worldRotation[ins.id], child);
			child = m_data.nextSibling[child.id];
		}
	}

	void TransformComponentSystem::parent(Component parent, Component child)
	{
		auto firstChild = m_data.firstChild[parent.id];
		if (!isValid(firstChild))
		{
			// This is the first child
			m_data.firstChild[parent.id] = child;
			m_data.nextSibling[child.id] = Component();
			m_data.prevSibling[child.id] = Component();
			m_data.firstChild[child.id] = Component();
			m_data.parent[child.id] = parent;
		}
		else
		{
			// There are more childs, find the last one then add this one as next
			auto lastValidSibling = firstChild;
			auto nextSibling = m_data.nextSibling[lastValidSibling.id];

			while (isValid(nextSibling))
			{
				lastValidSibling = nextSibling;
				nextSibling = m_data.nextSibling[lastValidSibling.id];
			}

			// Add the new child next the hierarchy
			m_data.nextSibling[lastValidSibling.id] = child;
			m_data.prevSibling[child.id] = lastValidSibling;
			m_data.parent[child.id] = parent;
			m_data.firstChild[child.id] = Component();
			m_data.nextSibling[child.id] = Component();

		}
	}

	bool TransformComponentSystem::isValid(Component ins)
	{
		if (ins.id != -1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	TransformComponentSystem::Component TransformComponentSystem::getComponent(Entity e)
	{
		auto it = m_map.find(e.m_id);
		if (it != m_map.end())
		{
			Component ins;
			ins.id = it->second;
			return ins;
		}

		Component ins;
		ins.id = -1;
		return ins;
	}

	TransformComponentSystem::Component TransformComponentSystem::addComponent(Entity e)
	{
		if (m_data.numOccupied >= m_data.numAllocated)
		{
			allocate(m_data.numAllocated * 2);
		}

		Component ins;
		ins.id = m_data.numOccupied++;

		m_data.entity[ins.id] = e;
		m_data.parent[ins.id] = Component();
		m_data.firstChild[ins.id] = Component();

		m_map[e.m_id] = ins.id;

		return ins;
	}
}
