#include <EGE\Model.hpp>

#include <stdio.h>

using namespace vmath;

namespace EGE
{
	Model::Model()
		: numVerts_(0)
		, modelMatrix_(mat4::identity())
		, position_(vec3(0,0,0))
		, rotation_(vec3(0, 0, 0))
	{

	}

	Model::~Model()
	{
		
	}

	void Model::create(const char* fileName)
	{
		// The imported mesh instance
		Mesh* importMesh = new Mesh();
		// Read from file
		FILE* file;
		file = fopen(fileName, "rb");
		// Read vertices
		fread(&importMesh->numVertices, sizeof(int), 1, file);
		importMesh->vertices = new float[importMesh->numVertices * 3];
		fread(importMesh->vertices, sizeof(float), importMesh->numVertices * 3, file);
		// Read vertex colors
		importMesh->vertexColors = new float[importMesh->numVertices * 4];
		fread(importMesh->vertexColors, sizeof(float), importMesh->numVertices * 4, file);
		// Read indices
		fread(&importMesh->numIndices, sizeof(int), 1, file);
		importMesh->indices = new int[importMesh->numIndices];
		fread(importMesh->indices, sizeof(int), importMesh->numIndices, file);
		fclose(file);

		numVerts_ = importMesh->numVertices;
		numIndices_ = importMesh->numIndices;

		// Index buffer
		// NOTE: Not currently used
		glGenBuffers(1, &indexBuffer_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * importMesh->numIndices, importMesh->indices, GL_STATIC_DRAW);

		// Vertex buffer
		glGenBuffers(1, &vertexBuffer_);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * numVerts_, importMesh->vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(0);

		// Color buffer
		glGenBuffers(1, &colorBuffer_);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * numVerts_, importMesh->vertexColors, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(1);

		delete importMesh;
	}

	void Model::update()
	{
		modelMatrix_ = translate<float>(position_) * rotate<float>(rotation_[0], rotation_[1], rotation_[2]);
	}

	void Model::render()
	{
		glDrawArrays(GL_TRIANGLES, 0, numVerts_);
	}
}