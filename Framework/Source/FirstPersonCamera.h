//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#pragma once

#include "Camera.h"

class FirstPersonCamera : public Camera
{
public:
	FirstPersonCamera(glm::vec3 position);
	virtual ~FirstPersonCamera();

	virtual void Update(float dt);

	virtual glm::mat4 GetViewMatrix() const;

private:
	glm::vec3 mPosition;
	glm::vec3 mLookAt;
	glm::vec3 mUp;

	float mDisplacementSpeed;
	float mAngularSpeed;
	float mTiltAngularSpeedAdjustment;
	float mScrollDisplacementSpeedAdjustment;
	float mSlowDownFactor;
};
