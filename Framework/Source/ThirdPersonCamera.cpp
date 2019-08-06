#include "ThirdPersonCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace glm;

ThirdPersonCamera::ThirdPersonCamera(PlayerBubbleGroup* playerBubbleGroup) :
	mPlayerBubbleGroup(playerBubbleGroup)
{
}

glm::mat4 ThirdPersonCamera::GetViewMatrix() const
{
	mat4 viewMatrix = glm::lookAt(
		CalculateEyeVector(),
		mPlayerBubbleGroup->GetCenterOfMass(),
		mPlayerBubbleGroup->GetUp());
	return viewMatrix;
}

glm::vec3 ThirdPersonCamera::CalculateEyeVector() const
{
	// V / (4piR^3/3) == 1 -> zoomed out
	// V / (4piR^3/3) approaches 0 -> zoomed in
	float radius = mPlayerBubbleGroup->GetGroupRadius();
	float tween;
	if (radius > 0.0f)
	{
		float volume = mPlayerBubbleGroup->GetGroupVolume();
		tween = volume / (4.0f * (float)M_PI * pow(radius, 3.0f) / 3.0f);
		tween = pow(tween, 1.0f / 3.0f);
	}
	else
	{
		tween = 1.0f;
	}

	vec3 value = mPlayerBubbleGroup->GetCenterOfMass() - (7.5f * tween + 2.5f) * mPlayerBubbleGroup->GetGroupRadius() * mPlayerBubbleGroup->GetLookAt();
	return value;
}