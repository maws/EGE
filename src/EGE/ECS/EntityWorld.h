#pragma once

#include <EGE\ECS\EntityManager.h>

namespace EGE
{
	class TransformComponentSystem;
	class MeshResourceComponentSystem;
	class CameraComponentSystem;
	class SimpleHeapAllocator;
	class EntityWorld
	{
	public:
		EntityWorld();
		~EntityWorld();

		void tick();

		EntityManager* m_entityManager;
		CameraComponentSystem* m_cameraComponentSystem;
		TransformComponentSystem* m_transformComponentSystem;
		MeshResourceComponentSystem* m_meshResourceComponentSystem;

	private:
		SimpleHeapAllocator* m_allocator;

	};
}