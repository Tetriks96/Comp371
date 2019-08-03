#include "ThirdPersonCamera.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

ThirdPersonCamera::ThirdPersonCamera(PlayerBubbleGroup* playerBubbleGroup) :
	mPlayerBubbleGroup(playerBubbleGroup)
{
}

glm::mat4 ThirdPersonCamera::GetViewMatrix() const
{
	mat4 viewMatrix = glm::lookAt(
		mPlayerBubbleGroup->GetCenterOfMass() - 10.0f * mPlayerBubbleGroup->GetGroupRadius() * mPlayerBubbleGroup->GetLookAt(),
		mPlayerBubbleGroup->GetCenterOfMass(),
		mPlayerBubbleGroup->GetUp());
	return viewMatrix;
}