#pragma once

#include <EGE\Application.hpp>
#include <EGE\Camera.hpp>
#include <EGE\Shader.hpp>
#include <gmtl\gmtl.h>
#include <vmath.h>

namespace EGE
{
	class Model
	{
	public:
		Model();
		~Model();

		/// \brief Create a model instance from filename
		/// \param[in] fileName The relative file name
		/// \remark The model needs to be in e3m format
		void create(const char* fileName);

		/// \brief Create a model based out of a vertex array
		void create(const float* verts);

		/// \brief Updates this models transformation matrices
		void update();

		/// \brief Renders this model using GL_TRIANGLES
		void render(const Camera& camera, const RenderProgram& program);

		/// \return vec3 reference position
		vmath::vec3& getPosition(){ return position_; }

		/// \return vec3 reference rotation
		vmath::vec3& getRotation() { return rotation_; }
		
		/// \return const mat4 reference model matrix
		const vmath::mat4& getTransformMatrix() const { return transformMatrix_; }

	private:
		vmath::vec3 position_;
		vmath::vec3 rotation_;
		vmath::mat4 transformMatrix_;
		unsigned numVerts_;
		GLuint vertexBuffer_;
		GLuint colorBuffer_;

		/// \brief Simple mesh structure used for import/exporting
		struct Mesh
		{
			Mesh()
			{
				vertices = nullptr;
				indices = nullptr;
			}

			~Mesh()
			{
				if (vertices) delete vertices;
				if (indices) delete indices;
			}

			int numVertices;
			float* vertices;
			float* vertexColors;
			int numIndices;
			int* indices;
		};

	};
}