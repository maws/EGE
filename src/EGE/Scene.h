#pragma once

namespace EGE
{
	class EntityWorld;
	class Scene
	{
	public:
		virtual void init(EntityWorld*) = 0;
		virtual void destroy() = 0;
		virtual void update(float dt) = 0;

		EntityWorld* m_world;
	};
}