#include <EGE\Model.hpp>

#include <stdio.h>

using namespace vmath;

namespace EGE
{
	Model::Model()
		: vertices_(nullptr)
		, numVerts_(0)
		, modelMatrix_(mat4::identity())
		, position_(vec3(0,0,0))
		, rotation_(vec3(0, 0, 0))
	{

	}

	Model::~Model()
	{
		delete vertices_;
	}

	void Model::create(const char* fileName)
	{
		// Read from file
		FILE* file;
		file = fopen(fileName, "rb");
		fread(&numVerts_, sizeof(unsigned), 1, file);
		vertices_ = new float[numVerts_];
		fread(vertices_, sizeof(float), numVerts_, file);
		fclose(file);

		static const GLfloat vertices[] =
		{
			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f
		};
		unsigned numVerts = sizeof(vertices) / sizeof(float);

		float b[72];
		for (int i = 0; i < 72; i++)
		{
			b[i] = vertices_[i];
		}

		// Create vertex buffer
		glGenBuffers(1, &vertexBuffer_);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
		glBufferData(GL_ARRAY_BUFFER, numVerts_ * sizeof(float), vertices_, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(0);
	}

	void Model::update()
	{
		modelMatrix_ = modelMatrix_ * translate<float>(position_) * rotate<float>(rotation_[0], rotation_[1], rotation_[2]);
	}

	void Model::render()
	{
		glDrawArrays(GL_TRIANGLES, 0, 24);
	}
}