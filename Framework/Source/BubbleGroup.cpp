#include "BubbleGroup.h"
#include "EventManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace glm;

BubbleGroup::BubbleGroup(vec3 centerOfMass, float volume)
{
	mCenterOfMass = centerOfMass;
	mGroupVolume = volume;
	Bubble* initialBubble = new Bubble(vec3(0.0f), volume, vec3(0.0f, 1.0f, 1.0f));
	mGroupRadius = initialBubble->GetRadius();
	mMoveTowards = vec3(0.0f);
	mBubbles.push_back(initialBubble);
	mLastSplitTime = -1.0;
}

BubbleGroup::~BubbleGroup()
{

}

void BubbleGroup::Update(float dt)
{
	for (vector<Bubble*>::iterator it = mBubbles.begin(); it < mBubbles.end(); it++)
	{
		if (*it == nullptr)
		{
			continue;
		}
		(*it)->Update(dt, mMoveTowards, mCenterOfMass - (*it)->GetPosition());
	}

	// Remove null bubbles
	for (vector<Bubble*>::iterator it = mBubbles.begin(); it < mBubbles.end();)
	{
		if (*it == nullptr)
		{
			it = mBubbles.erase(it);
		}
		else
		{
			it++;
		}
	}

	mCenterOfMass = CalculateCenterOfMass();
	mGroupVolume = CalculateGroupVolume();
	mGroupRadius = CalculateGroupRadius();
}

void BubbleGroup::Draw()
{
	for (vector<Bubble*>::iterator it = mBubbles.begin(); it < mBubbles.end(); ++it)
	{
		(*it)->Draw();
	}
}

void BubbleGroup::Split(float dt)
{
	if (length(mMoveTowards) <= 0.0f || EventManager::GetGameTime() - mLastSplitTime < 1.0)
	{
		// Split is undefined if the bubble group is not moving
		// Two splits are not allowed within 1 second of eachother
		return;
	}

	mLastSplitTime = EventManager::GetGameTime();

	int currentSize = mBubbles.size();
	mBubbles.reserve(2 * currentSize);

	for (int i = 0; i < currentSize; i++)
	{
		Bubble* newBubble = mBubbles[i]->Split(dt, mMoveTowards);
		if (newBubble != nullptr)
		{
			mBubbles.push_back(newBubble);
		}
	}
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

float BubbleGroup::CalculateGroupVolume()
{
	float groupVolume = 0.0f;
	for (vector<Bubble*>::iterator it = mBubbles.begin(); it < mBubbles.end(); ++it)
	{
		groupVolume += (*it)->GetVolume();
	}
	return groupVolume;
}