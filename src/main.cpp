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
		camera->create(55.0f, windowInfo_.width / windowInfo_.height, 0.1f, 1024.0f);
		camera->getPosition() = vmath::vec3(0.0f, 0.0f, -5.0f);
	}

	virtual void shutdown() override
	{
		delete camera;
		delete model;
		delete renderProgram;
	}

	float oldX = 0.0f;
	float newX = 0.0f;
	float oldY = 0.0f;
	float newY = 0.0f;
	float changeX = 0.0f;
	float changeY = 0.0f;
	virtual void onMouseMove(int x, int y) override
	{
		int clamp = 50;
		// X
		oldX = newX;
		newX = x;
		float deltaX = newX - oldX;
		if (deltaX > clamp) deltaX = clamp;
		if (deltaX < -clamp) deltaX = -clamp;
		// Y
		oldY = newY;
		newY = y;
		float deltaY = newY - oldY;
		if (deltaY > clamp) deltaY = clamp;
		if (deltaY < -clamp) deltaY = -clamp;

		camera->getRotation()[0] += deltaY / 10;
		camera->getRotation()[1] += deltaX / 10;
	}

	virtual void onKey(int key, int action) override
	{
		if (action)
		{
			
		}
	}

	virtual void render(double time) override
	{
		// input
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
		GLuint color = glGetAttribLocation(renderProgram->getProgram(), "color");
		GLuint mvMatrix = glGetUniformLocation(renderProgram->getProgram(), "mvMatrix");
		GLuint projMatrix = glGetUniformLocation(renderProgram->getProgram(), "projMatrix");
		glVertexAttrib3fv(color, vmath::vec4(1.0f, .0f, .0f, 1.0f));
		glUniformMatrix4fv(mvMatrix, 1, false, modelViewMatrix);
		glUniformMatrix4fv(projMatrix, 1, false, camera->getProjMatrix());

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		model->render();
		
	}

	EGE::Camera* camera;
	EGE::Model* model;
	EGE::RenderProgram* renderProgram;
	GLuint vertexArray;
private:

};

int main()
{
	App app;
	app.run(&app);

	return 0;
}
