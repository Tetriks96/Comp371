#include "BubbleGroup.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace glm;

BubbleGroup::BubbleGroup(vec3 centerOfMass, float volume)
{
	mCenterOfMass = centerOfMass;
	mVolume = volume;
	Bubble* initialBubble = new Bubble(vec3(0.0f), mVolume, vec3(0.0f, 1.0f, 1.0f));
	mRadius = initialBubble->GetRadius();
	mMoveTowards = vec3(0.0f);
	mBubbles.push_back(initialBubble);
}

BubbleGroup::~BubbleGroup()
{

}

void BubbleGroup::Update(float dt)
{
	if (length(mMoveTowards) > 0.0f)
	{
		mCenterOfMass += 20 * dt * normalize(mMoveTowards);

		for (vector<Bubble*>::iterator it = mBubbles.begin(); it < mBubbles.end(); ++it)
		{
			(*it)->SetPosition(mCenterOfMass);
		}
	}
}

void BubbleGroup::Draw()
{
	for (vector<Bubble*>::iterator it = mBubbles.begin(); it < mBubbles.end(); ++it)
	{
		(*it)->Draw();
	}
}

void BubbleGroup::Split()
{

}