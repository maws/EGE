#include <EGE\Camera.hpp>

using namespace gmtl;
using namespace vmath;

namespace EGE
{
	Camera::Camera()
		: transformMatrix_(mat4::identity())
		, projMatrix_(mat4::identity())
		, viewMatrix_(mat4::identity())
		, position_(0)
		, rotation_(0)
	{

	}

	Camera::~Camera()
	{

	}

	void Camera::create(float fovy, float aspect, float nearPlane, float farPlane)
	{
		projMatrix_ = perspective(fovy, aspect, nearPlane, farPlane);
	}

	void Camera::update()
	{
		mat4 trans = translate<float>(position_);
		mat4 rot = rotate<float>(rotation_[0], rotation_[1], rotation_[2]);
		transformMatrix_ = trans * rot;

		vec3 at = vec3(transformMatrix_[2][0], transformMatrix_[2][1], transformMatrix_[2][2]);
		vec3 up = vec3(transformMatrix_[1][0], transformMatrix_[1][1], transformMatrix_[1][2]);
		vec3 pos = vec3(transformMatrix_[3][0], transformMatrix_[3][1], transformMatrix_[3][2]);
		viewMatrix_ = lookat<float>(pos, pos + at, up);
	}
}