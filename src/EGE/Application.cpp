#include <EGE/Application.hpp>

#include <vmath.h>
#include <EGE/Input.h>
#include <EGE/ECS/EntityWorld.h>
#include <EGE/ECS/MeshResourceComponentSystem.h>
#include <EGE/ECS/TransformComponentSystem.h>
#include <EGE/ECS/CameraComponentSystem.h>
#include <EGE/Resources.h>
#include <EGE/MeshResource.hpp>
#include <EGE/OpenGLContext.hpp>
#include <EGE/ECS/Util.h>

using namespace vmath;

namespace EGE
{
	Application* Application::m_instance = nullptr;

	Application::Application()
	{

	}

	Application::~Application()
	{
		
	}

	void Application::init()
	{
		m_world = new EntityWorld();
		m_ctx.init();

		int w, h;
		m_ctx.getWindowSize(&w, &h);

		auto cameraSystem = m_world->m_cameraComponentSystem;
		m_cameraEntity = m_world->m_entityManager->createEntity();
		auto camera = cameraSystem->addComponent(m_cameraEntity);
		cameraSystem->setProjectionMatrix(camera, perspective(55.0f, static_cast<float>(w / h), 0.1f, 2048));
		cameraSystem->setViewMatrix(camera, mat4::identity());

		auto cameraTransform = m_world->m_transformComponentSystem->addComponent(m_cameraEntity);
		m_world->m_transformComponentSystem->setLocalPosition(cameraTransform, vec3(0, 0, 0));
		m_world->m_transformComponentSystem->setLocalRotation(cameraTransform, vec3(0, 0, 0));
	}

	Application* Application::getInstance()
	{
		if (m_instance)
		{
			return m_instance;
		}
		else
		{
			m_instance = new Application();
			return m_instance;
		}
	}

	void Application::run()
	{
		bool running = true;
		while (running)
		{
			static float oldTime = 0.0f;
			static float newTime = 0.0f;
			oldTime = newTime;
			newTime = static_cast<float>(m_ctx.getTime());
			float dt = newTime - oldTime;

			m_world->tick();
			m_scenes.back()->update(dt);
			render();
			m_ctx.swapBuffers();
			m_ctx.pollEvents();

			auto& input = *Input::getInstance();
			running &= input.getKey(GLFW_KEY_ESCAPE) == GLFW_RELEASE;
			running &= (glfwWindowShouldClose(m_ctx.m_window) != GL_TRUE);
		}
	}

	void Application::render()
	{
		CameraComponentSystem* cameraSystem = m_world->m_cameraComponentSystem;
		MeshResourceComponentSystem* meshComponentSystem = m_world->m_meshResourceComponentSystem;
		TransformComponentSystem* transformComponentSystem = m_world->m_transformComponentSystem;

		auto camera = cameraSystem->getComponent(m_cameraEntity);
		const auto& projMatrix = cameraSystem->getProjectionMatrix(camera);
		const auto& viewMatrix = cameraSystem->getViewMatrix(camera);

		// Prepare render
		glClearBufferfv(GL_COLOR, 0, vmath::vec4(0, 0, 0, 1));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto& meshComponents = Util::getComponents(*meshComponentSystem);

		for (auto meshComponent : meshComponents)
		{
			Entity entity = meshComponentSystem->getEntity(meshComponent);
			MeshResource* meshResource = meshComponentSystem->getMeshResource(meshComponent);
			auto transform = transformComponentSystem->getComponent(entity);

			GLuint shaderProjMatrix = glGetUniformLocation(m_ctx.m_defaultShader->getProgram(), "projMatrix");
			glUniformMatrix4fv(shaderProjMatrix, 1, false, (float*)&projMatrix);

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			auto pos = transformComponentSystem->getWorldPosition(transform);
			auto rot = transformComponentSystem->getWorldRotation(transform);

			mat4 worldMatrix = mat4::identity();
			mat4 translation = translate<float>(pos);
			mat4 rotation = rotate<float>(rot[0], rot[1], rot[2]);
			worldMatrix = translation * rotation;
			mat4 modelViewMatrix = viewMatrix * worldMatrix;

			glBindBuffer(GL_ARRAY_BUFFER, meshResource->m_vertexBuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, meshResource->m_colorBuffer);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(1);
			GLuint shaderMvMatrix = glGetUniformLocation(m_ctx.m_defaultShader->getProgram(), "mvMatrix");
			shaderProjMatrix = glGetUniformLocation(m_ctx.m_defaultShader->getProgram(), "projMatrix");
			glUniformMatrix4fv(shaderMvMatrix, 1, false, (float*)&modelViewMatrix);
			glUniformMatrix4fv(shaderProjMatrix, 1, false, (float*)&projMatrix);
			glDrawArrays(GL_TRIANGLES, 0, meshResource->m_numVerts);
		}
	}

	void Application::PopScene()
	{
		auto scene = m_scenes.back();
		if (scene)
		{
			scene->destroy();
			m_scenes.pop_back();
			delete scene;
		}
		else
		{
			printf("Tried to pop scene when no scenes were in the stack. \n");
		}
	}

	void Application::PushScene(Scene* scene)
	{
		scene->init(m_world);
		m_scenes.push_back(scene);
	}
}