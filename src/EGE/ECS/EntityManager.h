#pragma once

#include <array>
#include <vector>

#include <EGE\ECS\Entity.h>

namespace EGE
{
	class EntityManager
	{
	public:
		EntityManager();

		Entity createEntity();
		void removeEntity(Entity entity);
		bool alive(Entity entity) const;

	private:
		std::vector<unsigned char> m_entityGeneration;
		std::vector<unsigned> m_freeEntities;
	};
}
