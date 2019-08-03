#include "BubbleGroup.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace glm;

BubbleGroup::BubbleGroup(vec3 centerOfMass, float volume)
{
	mCenterOfMass = centerOfMass;
	mGroupVolume = volume;
	Bubble* initialBubble = new Bubble(vec3(0.0f), mGroupVolume, vec3(0.0f, 1.0f, 1.0f));
	mGroupRadius = initialBubble->GetRadius();
	mMoveTowards = vec3(0.0f);
	mBubbles.push_back(initialBubble);
}

BubbleGroup::~BubbleGroup()
{

}

void BubbleGroup::Update(float dt)
{
	for (vector<Bubble*>::iterator it = mBubbles.begin(); it < mBubbles.end(); ++it)
	{
		(*it)->Update(dt, mMoveTowards, vec3(0.0f), false);
	}
	mCenterOfMass = (mBubbles[0]->GetPosition());
	mGroupVolume = (mBubbles[0]->GetVolume());
	mGroupRadius = (mBubbles[0]->GetRadius());
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