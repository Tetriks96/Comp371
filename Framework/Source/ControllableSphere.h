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
	glm::vec3 GetPosition();
	glm::vec3 GetLookAt();
	glm::vec3 GetUp();

	float GetRadius();

private:
	void Move(float dt);
	void HandleCollisions();

	SphereModel* mModel;
	glm::vec3 mLookAt;
	glm::vec3 mUp;
	float mDisplacementEnergy;
	float mAngularSpeed;
	float mSlowDownFactor;
	float mTiltAngularSpeedAdjustment;
	float mScrollDisplacementSpeedAdjustment;
};