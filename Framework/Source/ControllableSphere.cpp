#include "ControllableSphere.h"
#include "EventManager.h"
#include "World.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace glm;
using namespace std;

ControllableSphere::ControllableSphere(SphereModel* sphereModel) :
	mLookAt(0.0f, 0.0f, 1.0f),
	mUp(0.0f, 1.0f, 0.0f),
	mDisplacementEnergy(20.0f),
	mAngularSpeed(0.75f),
	mSlowDownFactor(0.1f),
	mTiltAngularSpeedAdjustment(5.0f),
	mScrollDisplacementSpeedAdjustment(5.0f)
{
	mSphereModel = sphereModel;
}

ControllableSphere::~ControllableSphere()
{
	if (mSphereModel != nullptr)
	{
		delete mSphereModel;
	}
}

void ControllableSphere::Update(float dt)
{
	Move(dt);
	HandleCollisions();
}

void ControllableSphere::HandleCollisions()
{
	World* world = World::GetInstance();
	vector<SphereModel*>* sphereModels = world->GetSphereModels();

	for (vector<SphereModel*>::iterator it = sphereModels->begin(); it < sphereModels->end(); ++it)
	{
		if (*it == nullptr)
		{
			continue;
		}
		vec3 position = (*it)->GetPosition();
		float radius = (*it)->GetScaling()[0]; // just take x value. Assume all models are spheres for now...
		float dist = distance(mSphereModel->GetPosition(), position);
		float myRadius = mSphereModel->GetScaling()[0];

		if (dist - std::max(radius, myRadius) < 0)
		{
			// Collision!
			if (myRadius > radius)
			{
				float myVolume = 4 * (float)M_PI * pow(myRadius, 3) / 3;
				float volume = 4 * (float)M_PI * pow(radius, 3) / 3;

				myVolume += volume;
				myRadius = pow(3 * myVolume / (4 * (float)M_PI), 1.0f / 3);

				mSphereModel->SetScaling(vec3(myRadius));

				delete *it;
				*it = nullptr;
			}
		}
	}
}

void ControllableSphere::Move(float dt)
{
	// Prevent from having the camera move only when the cursor is within the windows
	EventManager::DisableMouseCursor();

	// The Camera moves based on the User inputs
	// - You can access the mouse motion with EventManager::GetMouseMotionXY()
	// - For mapping A S D W, you can look in World.cpp for an example of accessing key states
	// - Don't forget to use dt to control the speed of the camera motion

	vec3 left = glm::cross(mUp, mLookAt);

	float adjustedAngularSpeed = mAngularSpeed;
	float adjustedDisplacementSpeed = mDisplacementEnergy / GetRadius();
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		adjustedAngularSpeed *= mSlowDownFactor;
		adjustedDisplacementSpeed *= mSlowDownFactor;
	}

	float horizontalMotion = -EventManager::GetMouseMotionX() * adjustedAngularSpeed;
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

	vec3 previousPosition = mSphereModel->GetPosition();

	if (length(displacement) > 0)
	{
		displacement = normalize(displacement);
		displacement *= dt * adjustedDisplacementSpeed;
	}

	//double scrollMotion = EventManager::GetMouseMotionScroll();
	//displacement += mLookAt * (float)scrollMotion * dt * mScrollDisplacementSpeedAdjustment * adjustedDisplacementSpeed;

	mSphereModel->SetPosition(previousPosition + displacement);
}

void ControllableSphere::Draw()
{
	mSphereModel->Draw();
}

vec3 ControllableSphere::GetPosition()
{
	return mSphereModel->GetPosition();
}

vec3 ControllableSphere::GetLookAt()
{
	return mLookAt;
}

vec3 ControllableSphere::GetUp()
{
	return mUp;
}

float ControllableSphere::GetRadius()
{
	return mSphereModel->GetScaling()[0];
}