#pragma once

#include "SphereModel.h"
#include <glm/fwd.hpp>

class ControllableSphere
{
public:
	ControllableSphere(SphereModel* model);
	~ControllableSphere();
	void Update(float dt);
	void Draw();

private:
	SphereModel* mModel;
	glm::vec3 mLookAt;
	glm::vec3 mUp;
	float mDisplacementSpeed;
	float mAngularSpeed;
	float mSlowDownFactor;
	float mTiltAngularSpeedAdjustment;
	float mScrollDisplacementSpeedAdjustment;
};