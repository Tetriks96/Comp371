#include "PlayerBubbleGroup.h"
#include "EventManager.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace glm;

PlayerBubbleGroup::PlayerBubbleGroup() : BubbleGroup(vec3(0.0f), 2.0f)
{
	mLookAt = vec3(0.0f, 0.0f, 1.0f);
	mUp = vec3(0.0f, 1.0f, 0.0f);
	mLookAroundAngularSpeed = 1.0f;
	mTiltAngularSpeed = 1.0f;
}

PlayerBubbleGroup::~PlayerBubbleGroup()
{

}

void PlayerBubbleGroup::Update(float dt)
{
	// Prevent from having the camera move only when the cursor is within the windows
	EventManager::DisableMouseCursor();

	// The Camera moves based on the User inputs
	// - You can access the mouse motion with EventManager::GetMouseMotionXY()
	// - For mapping A S D W, you can look in World.cpp for an example of accessing key states
	// - Don't forget to use dt to control the speed of the camera motion

	vec3 left = glm::cross(mUp, mLookAt);

	float horizontalMotion = -EventManager::GetMouseMotionX() * mLookAroundAngularSpeed;
	float verticalMotion = EventManager::GetMouseMotionY() * mLookAroundAngularSpeed;

	mat4 lookAtRotation(1.0f);
	mat4 upRotation(1.0f);

	// Horizontal motion
	lookAtRotation = rotate(lookAtRotation, horizontalMotion * dt * mLookAroundAngularSpeed, mUp);
	upRotation = rotate(upRotation, horizontalMotion * dt * mLookAroundAngularSpeed, mUp);

	// Vertical motion
	lookAtRotation = rotate(lookAtRotation, verticalMotion * dt * mLookAroundAngularSpeed, left);
	upRotation = rotate(upRotation, verticalMotion * dt * mLookAroundAngularSpeed, left);

	// Tilt motion
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_Q) == GLFW_PRESS)
	{
		upRotation = rotate(upRotation, -mTiltAngularSpeed * dt , mLookAt);
	}
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_E) == GLFW_PRESS)
	{
		upRotation = rotate(upRotation, mTiltAngularSpeed * dt, mLookAt);
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

	// Displacement
	vec3 displacement(vec3(0.0f));

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		displacement += mLookAt;
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		displacement -= mLookAt;
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		displacement += left;
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		displacement -= left;
	}

	mMoveTowards = displacement;

	BubbleGroup::Update(dt);
}