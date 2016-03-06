#pragma once

#include <vmath.h>
#include <EGE\ECS\EntityManager.h>

#include <unordered_map>

namespace EGE
{
	class Allocator;
	class EntityWorld;
	class TransformComponentSystem;
	class CameraComponentSystem
	{
	public:
		CameraComponentSystem(Allocator*, EntityWorld*);
		~CameraComponentSystem();

		struct Component
		{
			Component() : m_id(-1) {}
			int m_id;
		};

		struct Data
		{
			unsigned numOccupied;
			unsigned numAllocated;
			void* buffer;
			Entity* entity;
			vmath::mat4* projectionMatrix;
			vmath::mat4* viewMatrix;
		};
		Data m_data;

		std::unordered_map<unsigned, unsigned> m_entityComponentMap;

		Component addComponent(Entity e);
		Component getComponent(Entity e);
		void removeComponent(Component ins);
		bool isValid(Component ins);
		void removeDeadEntities(const EntityManager& em);
		void tick();

		void setProjectionMatrix(Component ins, const vmath::mat4& proj);
		void setViewMatrix(Component ins, const vmath::mat4& view);

		Entity getEntity(Component ins) { return m_data.entity[ins.m_id]; }
		vmath::mat4 getProjectionMatrix(Component ins) { return m_data.projectionMatrix[ins.m_id]; }
		vmath::mat4 getViewMatrix(Component ins) { return m_data.viewMatrix[ins.m_id]; }

	private:
		void allocate(size_t size);

		Allocator* m_allocator;
		TransformComponentSystem* m_transformSystem;
	};
}