#pragma once

#include <EGE\Application.hpp>
#include <vmath.h>

namespace EGE
{
	class Model
	{
	public:
		Model();
		~Model();

		void create(const char* fileName);
		void update();
		void render();
		vmath::vec3& getPosition(){ return position_; }
		vmath::vec3& getRotation() { return rotation_; }
		const vmath::mat4 getModelMatrix() const { return modelMatrix_; }

	private:
		vmath::vec3 position_;
		vmath::vec3 rotation_;
		vmath::mat4 modelMatrix_;
		float* vertices_;
		unsigned numVerts_;
		GLuint vertexBuffer_;

	};
}