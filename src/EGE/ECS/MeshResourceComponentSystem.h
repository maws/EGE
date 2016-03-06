#pragma

#include <unordered_map>

#include <EGE\ECS\EntityManager.h>

namespace EGE
{
	class MeshResource;
	class Allocator;
	class EntityWorld;
	class MeshResourceComponentSystem
	{
	public:
		MeshResourceComponentSystem(Allocator*, EntityWorld*);
		~MeshResourceComponentSystem();

		struct Component
		{
			Component() :m_id(-1){}
			int m_id;
		};

		struct ComponentData
		{
			unsigned numOccupied;
			unsigned numAllocated;
			void* buffer;

			Entity* entity;
			uintptr_t* meshResource;
		};
		ComponentData m_data;

		std::unordered_map<unsigned, unsigned> m_entityComponentMap;

		Component addComponent(Entity e);
		Component getComponent(Entity e);
		void removeComponent(Component ins);
		bool isValid(Component ins);
		void removeDeadEntities(const EntityManager& em);
		void tick(){}

		void setMeshResource(Component, MeshResource*);
		MeshResource* getMeshResource(Component ins) { return reinterpret_cast<MeshResource*>(m_data.meshResource[ins.m_id]); }
		Entity getEntity(Component ins){ return m_data.entity[ins.m_id]; }

	private:
		void allocate(size_t size);
		Allocator* m_allocator;
	};
}