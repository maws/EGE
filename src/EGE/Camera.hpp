#pragma once

#include <EGE\Application.hpp>
#include <vmath.h>

namespace EGE
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		void create(float fovy, float aspect, float nearPlane, float farPlane);
		void update();
		vmath::vec3& getPosition() { return position_; }
		vmath::vec3& getRotation() { return rotation_; }
		const vmath::mat4& getViewMatrix() const { return viewMatrix_; }
		const vmath::mat4& getProjMatrix() const { return projMatrix_; }

	private:
		vmath::vec3 position_;
		vmath::vec3 rotation_;
		vmath::mat4 viewMatrix_;
		vmath::mat4 projMatrix_;

	};
}