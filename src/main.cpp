#include <EGE\Application.hpp>
#include <vmath.h>
class App : public EGE::Application
{
public:
	App(){}
	~App(){}

protected:
	virtual void init() override
	{
		Application::init();
	}

	virtual void startup() override
	{
		
	}

	virtual void shutdown() override
	{

	}

	virtual void render(double time) override
	{
		glClearBufferfv(GL_COLOR, 0, vmath::vec4(1, 0, 0, 1));
	}

private:

};

int main()
{
	App app;
	app.run(&app);

	return 0;
}
