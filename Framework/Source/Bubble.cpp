#include "Bubble.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace glm;

Bubble::Bubble(vec3 position, float volume, vec3 color)
{
	mVolume = volume;
	mRadius = CalculateRadius(volume);
	mSphereModel = new SphereModel(position, mRadius, color);
	mVelocity = vec3(0.0f);
}

Bubble::~Bubble()
{
	if (mSphereModel != nullptr)
	{
		delete mSphereModel;
	}
}

void Bubble::Update(float dt, glm::vec3 moveTowards, vec3 gravity, bool split)
{
	if (length(moveTowards) > 0.0f)
	{
		float equilibriumSpeed = CalculateEquilibriumSpeed();
		mVelocity += equilibriumSpeed * normalize(moveTowards) * dt;
		if (length(mVelocity) > equilibriumSpeed)
		{
			mVelocity = equilibriumSpeed * normalize(mVelocity);
		}
	}
	vec3 currentPosition = mSphereModel->GetPosition();
	vec3 nextPosition = currentPosition + mVelocity * dt;
	mSphereModel->SetPosition(nextPosition);
}

float Bubble::CalculateEquilibriumSpeed()
{
	return 20.0f / mRadius;
}

void Bubble::Draw()
{
	mSphereModel->Draw();
}

void Bubble::SetVolume(float volume)
{
	mVolume = volume;
	mRadius = CalculateRadius(volume);
	mSphereModel->SetScaling(vec3(mRadius));
}

float Bubble::CalculateRadius(float volume)
{
	return pow((3.0f * mVolume) / (4.0f * (float)M_PI), (1.0f / 3.0f));
}