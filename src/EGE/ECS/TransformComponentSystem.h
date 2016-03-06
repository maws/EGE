#pragma once

#include <vmath.h>
#include <EGE\ECS\EntityManager.h>

#include <unordered_map>

namespace EGE
{
	class Allocator;
	class EntityWorld;
	class TransformComponentSystem
	{
	public:
		TransformComponentSystem(Allocator*, EntityWorld*);
		~TransformComponentSystem();

		struct Component
		{
			Component() : id(-1) {}
			int id;
		};

		struct ComponentData
		{
			unsigned numOccupied;
			unsigned numAllocated;
			void* buffer;
			Entity* entity;
			vmath::vec3* localPosition;
			vmath::vec3* localRotation;
			vmath::vec3* worldPosition;
			vmath::vec3* worldRotation;
			Component* parent;
			Component* firstChild;
			Component* nextSibling;
			Component* prevSibling;
		};
		ComponentData m_data;

		std::unordered_map<unsigned, unsigned> m_map;

		Component addComponent(Entity e);
		Component getComponent(Entity e);
		void removeComponent(Component ins);
		bool isValid(Component ins);
		void removeDeadEntities(const EntityManager& em);
		void parent(Component parent, Component child);
		void tick(){}

		void setLocalPosition(Component ins, const vmath::vec3& v);
		void setLocalRotation(Component ins, const vmath::vec3& v);

		vmath::vec3 getWorldPosition(Component ins) { return m_data.worldPosition[ins.id]; }
		vmath::vec3 getWorldRotation(Component ins) { return m_data.worldRotation[ins.id]; }
		Entity getEntity(Component ins){ return m_data.entity[ins.id]; }

	private:
		void allocate(size_t size);
		void destroyChildren(Component ins);
		void transformPosition(const vmath::vec3& parent, Component ins);
		void transformRotation(const vmath::vec3& parent, Component ins);

		Allocator* m_allocator;
	};
}

