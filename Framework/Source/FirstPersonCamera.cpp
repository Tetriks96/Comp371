//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#include "FirstPersonCamera.h"
#include "EventManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <GLFW/glfw3.h>
#include <algorithm>



using namespace glm;

FirstPersonCamera::FirstPersonCamera(glm::vec3 position, glm::vec3 lookAt) : Camera(),
	mPosition(position),
	mLookAt(lookAt),
	mUp(0.0f, 1.0f, 0.0f),
	mDisplacementSpeed(50.0f),
	mAngularSpeed(0.75f),
	mScrollDisplacementSpeedAdjustment(5.0f),
	mTiltAngularSpeedAdjustment(5.0f),
	mSlowDownFactor(0.1f)
{
	mLookAt = normalize(mLookAt);
}

FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::Update(float dt)
{

}

glm::mat4 FirstPersonCamera::GetViewMatrix() const
{
	return glm::lookAt(	mPosition, mPosition + mLookAt, mUp);
}
