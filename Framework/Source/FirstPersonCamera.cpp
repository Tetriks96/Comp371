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
	// Prevent from having the camera move only when the cursor is within the windows
	EventManager::DisableMouseCursor();

	// The Camera moves based on the User inputs
	// - You can access the mouse motion with EventManager::GetMouseMotionXY()
	// - For mapping A S D W, you can look in World.cpp for an example of accessing key states
	// - Don't forget to use dt to control the speed of the camera motion

	vec3 left = glm::cross(mUp, mLookAt);

	float adjustedAngularSpeed = mAngularSpeed;
	float adjustedDisplacementSpeed = mDisplacementSpeed;
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		adjustedAngularSpeed *= mSlowDownFactor;
		adjustedDisplacementSpeed *= mSlowDownFactor;
	}

	float horizontalMotion = - EventManager::GetMouseMotionX() * adjustedAngularSpeed;
	float verticalMotion = EventManager::GetMouseMotionY() * adjustedAngularSpeed;

	mat4 lookAtRotation(1.0f);
	mat4 upRotation(1.0f);

	// Horizontal motion
	lookAtRotation = rotate(lookAtRotation, horizontalMotion * dt * adjustedAngularSpeed, mUp);
	upRotation = rotate(upRotation, horizontalMotion * dt * adjustedAngularSpeed, mUp);
	
	// Vertical motion
	lookAtRotation = rotate(lookAtRotation, verticalMotion * dt * adjustedAngularSpeed, left);
	upRotation = rotate(upRotation, verticalMotion * dt * adjustedAngularSpeed, left);

	// Tilt motion
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_Q) == GLFW_PRESS)
	{
		upRotation = rotate(upRotation, -mTiltAngularSpeedAdjustment * dt * adjustedAngularSpeed, mLookAt);
	}
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_E) == GLFW_PRESS)
	{
		upRotation = rotate(upRotation, mTiltAngularSpeedAdjustment * dt * adjustedAngularSpeed, mLookAt);
	}

	// Apply Rotations
	vec4 lookAt4(mLookAt, 0.0f);
	lookAt4 = lookAtRotation * lookAt4;
	mLookAt = vec3(lookAt4);
	mLookAt = normalize(mLookAt);

	vec4 up4(mUp, 0.0f);
	up4 = upRotation * up4;
	mUp = vec3(up4);
	mUp = normalize(mUp);

	vec3 displacement(0.0f);
	// Displacements
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		displacement += mUp;
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		displacement -= mUp;
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		displacement += left;
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		displacement -= left;
	}

	if (length(displacement) > 0)
	{
		displacement = normalize(displacement);
		displacement *= dt * adjustedDisplacementSpeed;
		mPosition += displacement;
	}

	double scrollMotion = EventManager::GetMouseMotionScroll();
	mPosition += mLookAt * (float)scrollMotion * dt * mScrollDisplacementSpeedAdjustment * adjustedDisplacementSpeed;
}

glm::mat4 FirstPersonCamera::GetViewMatrix() const
{
	return glm::lookAt(	mPosition, mPosition + mLookAt, mUp);
}
