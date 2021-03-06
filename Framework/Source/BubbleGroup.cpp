#include "BubbleGroup.h"
#include "EventManager.h"
#include "SpikeBall.h"
#include "World.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace glm;

BubbleGroup::BubbleGroup(vec3 centerOfMass, float volume, vec3 color)
{
	mCenterOfMass = centerOfMass;
	mGroupVolume = std::max(0.0f, volume);
	mMoveTowards = vec3(0.0f);
	mLastSplitTime = -1.0;

	if (volume > 0.0f)
	{
		Bubble* initialBubble = new Bubble(centerOfMass, volume, color);
		mGroupRadius = initialBubble->GetRadius();
		mBubbles.push_back(initialBubble);
	}
	else
	{
		mGroupRadius = 0.0f;
	}
}

BubbleGroup::~BubbleGroup()
{
	for (vector<Bubble*>::iterator it = mBubbles.begin(); it < mBubbles.end(); it++)
	{
		delete *it;
	}
	mBubbles.clear();
}

void BubbleGroup::Update(float dt)
{
	for (int bb = 0; bb < (int)mBubbles.size(); bb++)
	{
		Bubble* bubble = mBubbles[bb];
		if (bubble == nullptr)
		{
			continue;
		}
		bubble->Update(dt, mMoveTowards, mCenterOfMass - bubble->GetPosition());
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
		if (*it == nullptr)
		{
			continue;
		}
		(*it)->Draw();
	}
}

void BubbleGroup::Split()
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
		if (mBubbles[i] == nullptr)
		{
			continue;
		}

		Bubble* newBubble = mBubbles[i]->Split(mMoveTowards);
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
		if (*it == nullptr)
		{
			continue;
		}

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
		return mCenterOfMass;
	}
}

float BubbleGroup::CalculateGroupRadius()
{
	// Length from center of mass to the farthest point on any of the group's bubbles
	float maxLength = 0.0f;
	for (vector<Bubble*>::iterator it = mBubbles.begin(); it < mBubbles.end(); ++it)
	{
		if (*it == nullptr)
		{
			continue;
		}

		float itsLength = distance(mCenterOfMass, (*it)->GetPosition()) + (*it)->GetRadius();
		maxLength = std::max(maxLength, itsLength);
	}

	return maxLength;
}

float BubbleGroup::CalculateGroupVolume()
{
	float groupVolume = 0.0f;
	for (vector<Bubble*>::iterator it = mBubbles.begin(); it < mBubbles.end(); ++it)
	{
		if (*it == nullptr)
		{
			continue;
		}

		groupVolume += (*it)->GetVolume();
	}
	return groupVolume;
}

void BubbleGroup::Pop()
{
	if (EventManager::GetGameTime() - mLastSplitTime < 1.0)
	{
		// Two splits are not allowed within 1 second of eachother
		return;
	}

	mLastSplitTime = EventManager::GetGameTime();

	int volume = (int)mGroupVolume;
	mBubbles.reserve(volume);

	int currentSize = mBubbles.size();

	for (int i = 0; i < currentSize; i++)
	{
		if (mBubbles[i] == nullptr)
		{
			continue;
		}

		vector<Bubble*>* newBubbles = mBubbles[i]->Pop();
		if (newBubbles != nullptr)
		{
			for (vector<Bubble*>::iterator it = newBubbles->begin(); it < newBubbles->end(); it++)
			{
				if (*it == nullptr)
				{
					continue;
				}

				mBubbles.push_back(*it);
			}
		}
	}
}