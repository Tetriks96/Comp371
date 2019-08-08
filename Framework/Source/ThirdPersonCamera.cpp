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
	vec3 eyeVector = CalculateEyeVector();
	vec3 centerOfMass = mPlayerBubbleGroup->GetCenterOfMass();
	vec3 up = mPlayerBubbleGroup->GetUp();
	mat4 viewMatrix = glm::lookAt(
		eyeVector,
		centerOfMass,
		up);
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

	vec3 playerCenterOfMass = mPlayerBubbleGroup->GetCenterOfMass();
	float playerRadius = mPlayerBubbleGroup->GetGroupRadius();
	if (playerRadius == 0.0f)
	{
		playerRadius = 1.0f;
	}
	vec3 playerLookAt = mPlayerBubbleGroup->GetLookAt();
	vec3 value = playerCenterOfMass - (7.5f * tween + 2.5f) * playerRadius * playerLookAt;

	return value;
}