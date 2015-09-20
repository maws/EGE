#include <EGE\Application.hpp>
#include <EGE\Model.hpp>
#include <EGE\Shader.hpp>
#include <EGE\Camera.hpp>
#include <EGE\Texture.h>
#include <vmath.h>
#include <gmtl\gmtl.h>
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

		monkeyModel = new EGE::Model;
		monkeyModel->create("assets/monkey.e3m");
		monkeyModel->getPosition() = vmath::vec3(0.0f, 200.0f, 0.0f);

		treeModel = new EGE::Model;
		treeModel->create("assets/tree.e3m");
		treeModel->getPosition() = vmath::vec3(10.0f, 200.0f, 200.0f);

		float scale = 1000.0f;
		float terrainVerts[] = 
		{
			-scale,	0.0f, scale,
			scale,	0.0f, scale,
			-scale,	0.0f, -scale,

			-scale, 0.0f, -scale,
			scale,	0.0f, scale, 
			scale,	0.0f, -scale
			
		};
		terrainModel = new EGE::Model;
		terrainModel->create(terrainVerts);

		GLuint vertexShader = EGE::loadShader("assets/vertex_shader.gl", GL_VERTEX_SHADER);
		GLuint fragmentShader = EGE::loadShader("assets/fragment_shader.gl", GL_FRAGMENT_SHADER);
		GLuint shaders[2] = { vertexShader, fragmentShader };
		renderProgram = new EGE::RenderProgram();
		renderProgram->create(shaders, 2);
		glUseProgram(renderProgram->getProgram());

		camera = new EGE::Camera();
		camera->create(55.0f, static_cast<float>(windowInfo_.width / windowInfo_.height), 0.1f, 1024.0f);

		texture = new EGE::Texture();
		texture->create("assets/test.png");
	}

	virtual void shutdown() override
	{
		delete camera;
		delete terrainModel;
		delete monkeyModel;
		delete treeModel;
		delete texture;
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

		camera->getRotation()[0] += moveY[0] * 0.07f;
		camera->getRotation()[1] += -moveX[0] * 0.07f;
	}

	virtual void onKey(int key, int action) override
	{
		
	}

	virtual void render(double time) override
	{
		// Grab basic inputs
		vmath::vec3 at = vmath::vec3(camera->getTransformMatrix()[2][0], camera->getTransformMatrix()[2][1], camera->getTransformMatrix()[2][2]);
		vmath::vec3 left = vmath::vec3(camera->getTransformMatrix()[0][0], camera->getTransformMatrix()[0][1], camera->getTransformMatrix()[0][2]);
		if (glfwGetKey(window_, 'W'))
			camera->getPosition() += at * 3;
		if (glfwGetKey(window_, 'A'))
			camera->getPosition() += left * 3;
		if (glfwGetKey(window_, 'S'))
			camera->getPosition() -= at * 3;
		if (glfwGetKey(window_, 'D'))
			camera->getPosition() -= left * 3;

		// Position
		monkeyModel->getRotation()[0] += 0.2f;
		monkeyModel->getRotation()[1] += 0.2f;

		//treeModel->getRotation()[0] += 0.5f;

		// Update transformations
		camera->update();
		monkeyModel->update();
		treeModel->update();

		// Render
		glClearBufferfv(GL_COLOR, 0, vmath::vec4(0, 0, 0, 1));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Send variables to shader
		GLuint projMatrix = glGetUniformLocation(renderProgram->getProgram(), "projMatrix");
		glUniformMatrix4fv(projMatrix, 1, false, (float*)&camera->getProjMatrix());

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);

		monkeyModel->render(*camera, *renderProgram);
		treeModel->render(*camera, *renderProgram);
		terrainModel->render(*camera, *renderProgram);
		
	}

	EGE::Camera* camera;
	EGE::Model* terrainModel;
	EGE::Model* monkeyModel;
	EGE::Model* treeModel;
	EGE::Texture* texture;
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
