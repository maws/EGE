#include <EGE\ECS\EntityWorld.h>

#include <EGE\ECS\Util.h>
#include <EGE\ECS\MeshResourceComponentSystem.h>
#include <EGE\ECS\TransformComponentSystem.h>
#include <EGE\ECS\CameraComponentSystem.h>
#include <EGE\Memory\Allocator.h>

namespace EGE
{
	EntityWorld::EntityWorld()
	{
		m_entityManager = new EntityManager();

		m_allocator = new SimpleHeapAllocator();

		m_transformComponentSystem = new TransformComponentSystem(m_allocator, this);
		m_cameraComponentSystem = new CameraComponentSystem(m_allocator, this);
		m_meshResourceComponentSystem = new MeshResourceComponentSystem(m_allocator, this);
	}

	EntityWorld::~EntityWorld()
	{
		delete m_entityManager;
		delete m_transformComponentSystem;
		delete m_meshResourceComponentSystem;

		delete m_allocator;
	}

	void EntityWorld::tick()
	{
		m_cameraComponentSystem->tick();
	}
}
