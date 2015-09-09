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

		/// \brief Create camera matrices
		/// param[in] fovy Vertical field of view
		/// param[in] aspect Aspect ratio
		/// param[in] nearPlane Near clip
		/// param[in] farPlane Far clip
		void create(float fovy, float aspect, float nearPlane, float farPlane);

		/// \brief Update view matrix
		void update();

		/// \return vec3 reference position
		vmath::vec3& getPosition() { return position_; }

		/// \return vec3 reference rotation
		vmath::vec3& getRotation() { return rotation_; }

		/// \return const mat4 reference view matrix
		const vmath::mat4& getViewMatrix() const { return viewMatrix_; }

		/// \return const mat4 reference projection matrix
		const vmath::mat4& getProjMatrix() const { return projMatrix_; }

	private:
		vmath::vec3 position_;
		vmath::vec3 rotation_;
		vmath::mat4 viewMatrix_;
		vmath::mat4 projMatrix_;

	};
}