#pragma once

#include "BubbleGroup.h"

class PlayerBubbleGroup : public BubbleGroup
{
public:
	PlayerBubbleGroup(float volume);
	~PlayerBubbleGroup();
	virtual void Update(float dt);
	glm::vec3 GetLookAt() { return mLookAt; }
	glm::vec3 GetUp() { return mUp; }
protected:
	glm::vec3 mLookAt;
	glm::vec3 mUp;
	float mLookAroundAngularSpeed;
	float mTiltAngularSpeed;
private:
};