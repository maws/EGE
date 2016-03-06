#pragma once

#include <vector>
#include <EGE\ECS\Entity.h>

namespace EGE
{
	namespace Util
	{
		template<typename T>
		inline std::vector<typename T::Component> getComponents(const T& system)
		{
			std::vector<T::Component> components;
			components.reserve(system.m_entityComponentMap.size());
			for (auto it : system.m_entityComponentMap)
			{
				T::Component ins;
				ins.m_id = it.second;
				components.push_back(ins);
			}
			return components;
		}

		template<typename T>
		inline std::vector<Entity> getEntities(const T& system)
		{
			std::vector<Entity> entities;
			entities.reserve(system.m_entityComponentMap.size());
			for (auto it : system.m_entityComponentMap)
			{
				Entity e;
				e.m_id = it.first;
				entities.push_back(e);
			}
			return entities;
		}
	}
}