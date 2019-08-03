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
	if (length(moveTowards) > 0.0f)
	{
		vec3 previousPosition = mSphereModel->GetPosition();
		vec3 newPosition = previousPosition + CalculateEquilibriumSpeed() * normalize(moveTowards) * dt;
		mSphereModel->SetPosition(newPosition);
	}

	// Handle Collisions
	vector<Bubble*>* bubbles = World::GetInstance()->GetBubbles();
	vector<Bubble*>::iterator it = bubbles->begin();
	while (it != bubbles->end())
	{
		vec3 itsPosition = (*it)->GetPosition();
		float itsRadius = (*it)->GetRadius();
		if (distance(GetPosition(), itsPosition) < std::max(mRadius, itsRadius))
		{
			// Collision!
			float itsVolume = (*it)->GetVolume();
			if (mVolume > itsVolume)
			{
				// Absorb it!
				float myNewVolume = mVolume + itsVolume;
				SetVolume(myNewVolume);
				it = bubbles->erase(it);
				continue;
			}
		}
		it++;
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
	return pow((3.0f * mVolume) / (4.0f * (float)M_PI), (1.0f / 3.0f));
}