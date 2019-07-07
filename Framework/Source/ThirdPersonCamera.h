#pragma once

#include "Camera.h"
#include "ControllableSphere.h"

class ControllableSphere;

class ThirdPersonCamera : public Camera
{
public:
	ThirdPersonCamera(ControllableSphere* controllableSphere);

	virtual void Update(float dt);
	virtual glm::mat4 GetViewMatrix() const;

private:
	ControllableSphere* mControllableSphere;
};