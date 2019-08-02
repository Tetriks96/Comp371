#pragma once

#include "Camera.h"
#include "PlayerBubbleGroup.h"

class ControllableSphere;

class ThirdPersonCamera : public Camera
{
public:
	ThirdPersonCamera(PlayerBubbleGroup* bubbleGroup);
	virtual void Update(float dt) {};
	virtual glm::mat4 GetViewMatrix() const;

private:
	PlayerBubbleGroup * mPlayerBubbleGroup;
};