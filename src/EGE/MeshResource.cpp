#include <EGE\MeshResource.hpp>

#include <stdio.h>
#include <vmath.h>

using namespace vmath;

namespace EGE
{
	void MeshResource::create(const std::string& path)
	{
		struct MeshData
		{
			unsigned numVerts;
			unsigned numIndices;
			float* verts;
			float* colors;
			int* indices;
		};

		FILE* file;
		if (file = fopen(path.c_str(), "rb"))
		{
			MeshData importData;
			
			fread(&importData.numVerts, sizeof(int), 1, file);
			importData.verts = new float[importData.numVerts * 3];
			fread(importData.verts, sizeof(float), importData.numVerts * 3, file);
			importData.colors = new float[importData.numVerts * 4];
			fread(importData.colors, sizeof(float), importData.numVerts * 4, file);
			fread(&importData.numIndices, sizeof(int), 1, file);
			importData.indices = new int[importData.numIndices];
			fread(importData.indices, sizeof(int), importData.numIndices, file);
			fclose(file);

			m_numVerts = importData.numVerts;

			glGenBuffers(1, &m_vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m_numVerts, importData.verts, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(0);

			glGenBuffers(1, &m_colorBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * m_numVerts, importData.colors, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(1);

			delete importData.colors;
			delete importData.verts;
			delete importData.indices;
		}
		else
		{
			printf("Could not find model %s \n", path);
		}
	}
}