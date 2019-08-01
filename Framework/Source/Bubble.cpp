#include "Bubble.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace glm;

Bubble::Bubble(vec3 position, float volume, vec3 color)
{
	mVolume = volume;
	mRadius = CalculateRadius(volume);
	mSphereModel = new SphereModel(position, mRadius, color);
}

Bubble::~Bubble()
{
	if (mSphereModel != nullptr)
	{
		delete mSphereModel;
	}
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