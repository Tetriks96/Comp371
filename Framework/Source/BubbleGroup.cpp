#include "BubbleGroup.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace glm;

BubbleGroup::BubbleGroup(vec3 centerOfMass, float volume)
{
	mCenterOfMass = centerOfMass;
	mVolume = volume;
	mRadius = pow((3.0f * mVolume) / (4.0f * (float)M_PI), 1.0f / 3.0f);
	mMoveTowards = vec3(0.0f);
	SphereModel* initialBubble = new SphereModel(vec3(0.0f), mVolume, vec3(0.0f, 1.0f, 1.0f));
	mSphereModels.push_back(initialBubble);
}

BubbleGroup::~BubbleGroup()
{

}

void BubbleGroup::Update(float dt)
{
	if (length(mMoveTowards) > 0.0f)
	{
		mCenterOfMass += 20 * dt * normalize(mMoveTowards);

		for (vector<SphereModel*>::iterator it = mSphereModels.begin(); it < mSphereModels.end(); ++it)
		{
			(*it)->SetPosition(mCenterOfMass);
			(*it)->SetVolume(mVolume);
		}
	}
}

void BubbleGroup::Draw()
{
	for (vector<SphereModel*>::iterator it = mSphereModels.begin(); it < mSphereModels.end(); ++it)
	{
		(*it)->Draw();
	}
}

void BubbleGroup::Split()
{

}