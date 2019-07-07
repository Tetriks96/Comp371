#include "ThirdPersonCamera.h"
#include "ControllableSphere.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

ThirdPersonCamera::ThirdPersonCamera(ControllableSphere* controllableSphere) :
	mControllableSphere(controllableSphere)
{
}

void ThirdPersonCamera::Update(float dt)
{
	
}

glm::mat4 ThirdPersonCamera::GetViewMatrix() const
{
	mat4 viewMatrix = glm::lookAt(
		mControllableSphere->GetPosition() - 10.0f * mControllableSphere->GetRadius() * mControllableSphere->GetLookAt(),
		mControllableSphere->GetPosition(),
		mControllableSphere->GetUp());
	return viewMatrix;
}