#include "Scenes\SceneWithEntities.h"

#include <vmath.h>

#include <EGE\Resources.h>
#include <EGE\ECS\MeshResourceComponentSystem.h>
#include <EGE\ECS\TransformComponentSystem.h>
#include <EGE\Input.h>

using namespace vmath;

void SceneWithEntities::init(EGE::EntityWorld* world)
{
	m_world = world;
	auto& em = *world->m_entityManager;
	m_player = em.createEntity();

	auto transformSystem = world->m_transformComponentSystem;
	auto meshSystem = world->m_meshResourceComponentSystem;

	auto transform = transformSystem->addComponent(m_player);
	transformSystem->setLocalPosition(transform, vec3(0, 0, 0));
	transformSystem->setLocalRotation(transform, vec3(0, 0, 0));

	auto mesh = meshSystem->addComponent(m_player);
	meshSystem->setMeshResource(mesh, EGE::Resources::getMesh("assets/player.e3m"));

	// Create some random entities for benchmark purposes
	for (int i = 0; i < 20; ++i)
	{
		Entity entity = em.createEntity();
		auto transform = transformSystem->addComponent(entity);
		transformSystem->setLocalPosition(transform, vec3(rand() % 200, 0, rand() % 200));
		transformSystem->setLocalRotation(transform, vec3(0, 0, 0));

		auto m = meshSystem->addComponent(entity);
		meshSystem->setMeshResource(m, EGE::Resources::getMesh("assets/fancy.e3m"));
	}
}

void SceneWithEntities::destroy()
{

}

void SceneWithEntities::update(float dt)
{
	auto ts = m_world->m_transformComponentSystem;
	auto transform = ts->getComponent(m_player);
	auto pos = ts->getWorldPosition(transform);
	auto rot = ts->getWorldRotation(transform);

	mat4 matrix = mat4::identity();
	matrix = translate(pos) * rotate(rot[0], rot[1], rot[2]);
	vec3 at = vec3(matrix[2][0], matrix[2][1], matrix[2][2]);
	vec3 left = vec3(matrix[0][0], matrix[0][1], matrix[0][2]);

	auto& input = *EGE::Input::getInstance();

	float speed = 50.0;
	if (input.getKey('W'))
		pos += (at * speed) * dt;
	else if (input.getKey('S'))
		pos -= (at * speed) * dt;
	if (input.getKey('D'))
		pos -= (left * speed) * dt;
	if (input.getKey('A'))
		pos += (left * speed) * dt;

	ts->setLocalPosition(transform, pos);
	ts->setLocalRotation(transform, rot);

	// follow with camera
	Entity camera;
	camera.m_id = 0;
	auto camTransform = ts->getComponent(camera);
	auto camPos = ts->getWorldPosition(camTransform);
	auto camRot = ts->getWorldRotation(camTransform);

	// Set pos/rot
	camPos = vec3(pos[0], pos[1] + 35.0f, pos[2] - 60.0f);
	camRot = vec3(20, 0, 0);

	ts->setLocalPosition(camTransform, camPos);
	ts->setLocalRotation(camTransform, camRot);
}