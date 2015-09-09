#include <EGE\Application.hpp>
#include <EGE\Model.hpp>
#include <EGE\Shader.hpp>
#include <EGE\Camera.hpp>
#include <vmath.h>
#include <cmath>

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
		glCreateVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		model = new EGE::Model;
		model->create("assets/cube.e3m");

		GLuint vertexShader = EGE::loadShader("assets/vertex_shader.gl", GL_VERTEX_SHADER);
		GLuint fragmentShader = EGE::loadShader("assets/fragment_shader.gl", GL_FRAGMENT_SHADER);
		GLuint shaders[2] = { vertexShader, fragmentShader };
		renderProgram = new EGE::RenderProgram();
		renderProgram->create(shaders, 2);
		glUseProgram(renderProgram->getProgram());

		camera = new EGE::Camera();
		camera->create(55.0f, static_cast<float>(windowInfo_.width / windowInfo_.height), 0.1f, 1024.0f);
		camera->getPosition() = vmath::vec3(0.0f, 0.0f, -5.0f);
	}

	virtual void shutdown() override
	{
		delete camera;
		delete model;
		delete renderProgram;
	}

	vmath::vec2 oldMouse = vmath::vec2(0, 0);
	vmath::vec2 newMouse = vmath::vec2(0, 0);
	virtual void onMouseMove(int x, int y) override
	{
		// Calculate position deltas
		oldMouse = newMouse;
		newMouse = vmath::vec2(static_cast<float>(x), static_cast<float>(y));
		vmath::vec2 deltaMouse = newMouse - oldMouse;

		// Clamp delta to keep away ridiculous values
		float clamp = 50.0f;
		vmath::vecN<float, 1> moveX = vmath::clamp<float, 1>(deltaMouse[0], -clamp, clamp);
		vmath::vecN<float, 1> moveY = vmath::clamp<float, 1>(deltaMouse[1], -clamp, clamp);

		camera->getRotation()[0] += moveY[0] / 10;
		camera->getRotation()[1] += moveX[0] / 10;
	}

	virtual void onKey(int key, int action) override
	{
		
	}

	virtual void render(double time) override
	{
		// Grab basic inputs
		if (glfwGetKey(window_, 'W'))
			camera->getPosition()[2] += 0.05f;
		if (glfwGetKey(window_, 'A'))
			camera->getPosition()[1] += 0.05f;
		if (glfwGetKey(window_, 'S'))
			camera->getPosition()[2] -= 0.05f;
		if (glfwGetKey(window_, 'D'))
			camera->getPosition()[1] -= 0.05f;

		// Position
		//model->getRotation()[0] += 0.2f;
		//model->getRotation()[1] += 0.5f;

		// Update transformations
		camera->update();
		model->update();

		vmath::mat4 modelViewMatrix = camera->getViewMatrix() * model->getModelMatrix();

		// Render
		glClearBufferfv(GL_COLOR, 0, vmath::vec4(0, 0, 0, 1));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Send variables to shader
		//GLuint color = glGetAttribLocation(renderProgram->getProgram(), "color");
		GLuint mvMatrix = glGetUniformLocation(renderProgram->getProgram(), "mvMatrix");
		GLuint projMatrix = glGetUniformLocation(renderProgram->getProgram(), "projMatrix");
		//glVertexAttrib3fv(color, vmath::vec4(1.0f, .0f, .0f, 1.0f));
		glUniformMatrix4fv(mvMatrix, 1, false, modelViewMatrix);
		glUniformMatrix4fv(projMatrix, 1, false, camera->getProjMatrix());

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		model->render();
		
	}

	EGE::Camera* camera;
	EGE::Model* model;
	EGE::RenderProgram* renderProgram;
	GLuint vertexArray;

};

int main()
{
	// Create and run example application
	App app;
	app.run(&app);

	return 0;
}
