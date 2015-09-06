#include <EGE\Camera.hpp>

using namespace vmath;

namespace EGE
{
	Camera::Camera()
		: viewMatrix_(mat4::identity())
		, projMatrix_(mat4::identity())
		, position_(vec3(0, 0, 0))
		, rotation_(vec3(0, 0, 0))
	{

	}

	Camera::~Camera()
	{

	}

	void Camera::create(float fovy, float aspect, float nearPlane, float farPlane)
	{
		viewMatrix_ = lookat<float>(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		projMatrix_ = perspective(fovy, aspect, nearPlane, farPlane);
	}

	void Camera::update()
	{
		viewMatrix_ = translate<float>(position_) * rotate<float>(rotation_[0], rotation_[1], rotation_[2]);
	}
}