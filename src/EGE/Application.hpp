#pragma once

#include <EGE/OpenGLContext.hpp>
#include <EGE/Scene.h>
#include <EGE/ECS/Entity.h>

#include <vector>

namespace EGE
{
	class Camera;
	class EntityWorld;
	class Application
	{
	public:
		virtual ~Application();

		static Application* getInstance();

		void init();
		void run();
		void render();
		void PopScene();
		void PushScene(Scene*);

		OpenGLContext m_ctx;

	private:
		static Application* m_instance;

		Application();
		Application(Application& other) {}
		void operator=(const Application &a){}

		Entity m_cameraEntity;
		EntityWorld* m_world;
		std::vector<Scene*> m_scenes;
	};
}