#include "Bubble.h"
#include "World.h"
#include <vector>
#include "EventManager.h"
#include "SpikeBall.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace glm;

Bubble::Bubble(vec3 position, float volume, vec3 color)
{
	mVolume = volume;
	mRadius = CalculateRadius(volume);
	mSphereModel = new SphereModel(position, mRadius, color);
	mDivisionVelocity = vec3(0.0f);
	mSplitFrom = nullptr;
	mLastSplitTime = 0.0;
}

Bubble::Bubble(vec3 position, float volume, vec3 color, glm::vec3 divisionVelocity, Bubble* splitFrom)
{
	mVolume = volume;
	mRadius = CalculateRadius(volume);
	mSphereModel = new SphereModel(position, mRadius, color);
	mDivisionVelocity = divisionVelocity;
	mSplitFrom = splitFrom;
	mLastSplitTime = EventManager::GetGameTime();
}

Bubble::~Bubble()
{
	if (mSphereModel != nullptr)
	{
		delete mSphereModel;
	}
}

void Bubble::Update(float dt, glm::vec3 moveTowards, vec3 gravity)
{
	if (length(moveTowards) == 0.0f &&
		length(gravity) == 0.0f &&
		length(mDivisionVelocity) == 0.0f)
	{
		return;
	}

	vec3 previousPosition = mSphereModel->GetPosition();
	UpdatePosition(dt, moveTowards, gravity);

	// Handle Collisions
	World* world = World::GetInstance();

	HandleCollisions(world->GetBubbles());

	vector<BubbleGroup*>* bubbleGroups = world->GetBubbleGroups();
	for (vector<BubbleGroup*>::iterator it = bubbleGroups->begin(); it < bubbleGroups->end(); it++)
	{
		vector<Bubble*>* bubbles = (*it)->GetBubbles();
		HandleCollisions(bubbles);
	}

	vector<SpikeBall*>* spikeBalls = world->GetSpikeBalls();

	for (vector<SpikeBall*>::iterator sb = spikeBalls->begin(); sb < spikeBalls->end(); sb++)
	{
		float distance = length(GetPosition() - (*sb)->GetPosition());
		if (distance < GetRadius() + (*sb)->GetRadius())
		{
			vector<Bubble*>* newBubbles = Pop();
			if (newBubbles != nullptr)
			{
				int w = 0;
				vector<Bubble*>* worldBubbles = world->GetBubbles();

				for (int n = 0; n < (int)newBubbles->size(); n++)
				{
					bool bubbleHandled = false;
					for (; w < (int)worldBubbles->size(); w++)
					{
						if ((*worldBubbles)[w] == nullptr)
						{
							(*worldBubbles)[w] = (*newBubbles)[n];
							bubbleHandled = true;
							break;
						}
					}
					if (!bubbleHandled)
					{
						world->GetBubbles()->push_back((*newBubbles)[n]);
					}
				}
			}
		}
	}
}

void Bubble::UpdatePosition(float dt, glm::vec3 moveTowards, glm::vec3 gravity)
{
	vec3 previousPosition = mSphereModel->GetPosition();
	if (length(moveTowards) > 0.0f)
	{
		moveTowards = normalize(moveTowards);
	}
	if (length(gravity) > 0.0f)
	{
		gravity = 0.05f * normalize(gravity) / mRadius;
	}
	vec3 newPosition = previousPosition + (CalculateEquilibriumSpeed() * (moveTowards + gravity)  + mDivisionVelocity) * dt;
	mSphereModel->SetPosition(newPosition);

	// Update division velocity
	mDivisionVelocity = std::max(0.0f, 1.0f - dt) * mDivisionVelocity;
}

float Bubble::CalculateEquilibriumSpeed()
{
	return CalculateEquilibriumSpeed(mRadius);
}

float Bubble::CalculateEquilibriumSpeed(float radius)
{
	return 10.0f / radius;
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
	return pow((3.0f * volume) / (4.0f * (float)M_PI), (1.0f / 3.0f));
}

void Bubble::HandleCollisions(vector<Bubble*>* bubbles)
{
	for (vector<Bubble*>::iterator it = bubbles->begin(); it < bubbles->end(); it++)
	{
		if (*it == nullptr || *it == this)
		{
			// Don't handle collisions with null bubbles or self
			continue;
		}
		vec3 itsPosition = (*it)->GetPosition();
		float itsRadius = (*it)->GetRadius();
		if (distance(GetPosition(), itsPosition) <= std::max(mRadius, itsRadius))
		{
			// Collision!

			if ((*it == mSplitFrom || (*it)->GetSplitFrom() == this || (*it)->GetSplitFrom() == mSplitFrom) && EventManager::GetGameTime() - mLastSplitTime < 1.0)
			{
				// Don't allow bubbles that just split from recolliding right away
				continue;
			}

			float itsVolume = (*it)->GetVolume();
			if (mVolume >= itsVolume)
			{
				// Absorb it!
				float myNewVolume = mVolume + itsVolume;
				SetVolume(myNewVolume);
				*it = nullptr;
				continue;
			}
		}
	}
}

Bubble* Bubble::Split(vec3 direction)
{
	// Split is undefined if no direction is provided
	// Split is not permitted for bubbles who's volume is smaller than 2
	if (length(direction) <= 0.0f || mVolume < 2.0f)
	{
		return nullptr;
	}

	SetVolume(mVolume / 2.0f);
	Bubble* newBubble = new Bubble(GetPosition(), mVolume, mSphereModel->GetColor(), mDivisionVelocity + CalculateEquilibriumSpeed() * normalize(direction), this);
	mLastSplitTime = EventManager::GetGameTime();
	return newBubble;
}

std::vector<Bubble*>* Bubble::Pop()
{
	if (mVolume < 2.0f)
	{
		return nullptr;
	}

	SetVolume(mVolume / 2);

	int popCount = (int)mVolume;
	float newBubbleVolume = mVolume / popCount;

	vector<Bubble*>* newBubbles = new vector<Bubble*>();
	for (int i = 0; i < popCount; i++)
	{
		Bubble* newBubble = new Bubble(GetPosition(), newBubbleVolume, mSphereModel->GetColor(), mDivisionVelocity + CalculateEquilibriumSpeed(newBubbleVolume) * World::GetRandomPositionInsideUnitSphere(), this);
		newBubbles->push_back(newBubble);
	}
	mLastSplitTime = EventManager::GetGameTime();
	return newBubbles;
}