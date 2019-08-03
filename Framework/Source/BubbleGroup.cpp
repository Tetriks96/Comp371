#include "BubbleGroup.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace glm;

BubbleGroup::BubbleGroup(vec3 centerOfMass, float volume)
{
	mCenterOfMass = centerOfMass;
	mGroupVolume = volume;
	Bubble* initialBubble1 = new Bubble(vec3(-10.0f, 0.0f, 0.0f), mGroupVolume, vec3(0.0f, 1.0f, 1.0f));
	Bubble* initialBubble2 = new Bubble(vec3(10.0f, 0.0f, 0.0f), mGroupVolume, vec3(0.0f, 1.0f, 1.0f));
	mGroupRadius = initialBubble1->GetRadius() + initialBubble2->GetRadius() + distance(initialBubble1->GetPosition(), initialBubble2->GetPosition());
	mMoveTowards = vec3(0.0f);
	mBubbles.push_back(initialBubble1);
	mBubbles.push_back(initialBubble2);
}

BubbleGroup::~BubbleGroup()
{

}

void BubbleGroup::Update(float dt)
{
	vector<Bubble*>::iterator it = mBubbles.begin();
	vector<Bubble*>::iterator end = mBubbles.end();
	while (it < end)
	{
		(*it)->Update(dt, mMoveTowards, mCenterOfMass - (*it)->GetPosition(), false);
		
		// If a collision happens between 2 bubbles in this group, the end of the iterator must be updated
		end = mBubbles.end();
		it++;
	}
	mCenterOfMass = CalculateCenterOfMass();
	mGroupVolume = mBubbles[0]->GetVolume() + mBubbles[1]->GetVolume();
	mGroupRadius = CalculateGroupRadius();
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

vec3 BubbleGroup::CalculateCenterOfMass()
{
	float sumOfms = 0.0f;
	float sumOfmXs = 0.0f;
	float sumOfmYs = 0.0f;
	float sumOfmZs = 0.0f;

	for (vector<Bubble*>::iterator it = mBubbles.begin(); it < mBubbles.end(); ++it)
	{
		float mass = (*it)->GetVolume();
		vec3 position = (*it)->GetPosition();

		sumOfms += mass;
		sumOfmXs += mass * position.x;
		sumOfmYs += mass * position.y;
		sumOfmZs += mass * position.z;
	}

	if (sumOfms > 0.0f)
	{
		return vec3(sumOfmXs, sumOfmYs, sumOfmZs) / sumOfms;
	}
	else
	{
		return vec3(0.0f);
	}
}

float BubbleGroup::CalculateGroupRadius()
{
	// Length from center of mass to the farthest point on any of the group's bubbles
	float maxLength = 0.0f;
	for (vector<Bubble*>::iterator it = mBubbles.begin(); it < mBubbles.end(); ++it)
	{
		float itsLength = distance(mCenterOfMass, (*it)->GetPosition()) + (*it)->GetRadius();
		maxLength = max(maxLength, itsLength);
	}

	return maxLength;
}