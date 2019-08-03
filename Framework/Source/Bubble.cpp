#include "Bubble.h"
#include "World.h"
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
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

void Bubble::Update(float dt, glm::vec3 moveTowards, vec3 gravity, bool split)
{
	// Update position
	vec3 previousPosition = mSphereModel->GetPosition();
	if (length(moveTowards) > 0.0f)
	{
		moveTowards = normalize(moveTowards);
	}
	vec3 newPosition = previousPosition + (CalculateEquilibriumSpeed() * moveTowards + gravity / 2.0f) * dt;
	mSphereModel->SetPosition(newPosition);

	// Handle Collisions
	World* world = World::GetInstance();

	HandleCollisions(world->GetBubbles());

	vector<BubbleGroup*>* bubbleGroups = world->GetBubbleGroups();
	for (vector<BubbleGroup*>::iterator it = bubbleGroups->begin(); it < bubbleGroups->end(); it++)
	{
		vector<Bubble*>* bubbles = (*it)->GetBubbles();
		HandleCollisions(bubbles);
	}
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
		if (distance(GetPosition(), itsPosition) < std::max(mRadius, itsRadius))
		{
			// Collision!
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