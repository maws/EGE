#include <EGE/Application.hpp>

#include "Scenes/SceneWithEntities.h"

int main()
{
	auto& app = *EGE::Application::getInstance();
	app.init();
	app.PushScene(new SceneWithEntities());
	app.run();

	return 0;
}
