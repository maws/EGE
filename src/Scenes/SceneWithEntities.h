#include <EGE/Scene.h>
#include <EGE/ECS/EntityWorld.h>
#include <EGE/RenderProgram.hpp>

class SceneWithEntities : public EGE::Scene
{
public:
	virtual void init(EGE::EntityWorld* world) override;
	virtual void destroy() override;
	virtual void update(float dt) override;

	EGE::EntityWorld* m_world;
	EGE::RenderProgram* m_shader;
	Entity m_player;
};