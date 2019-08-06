#include "AIBubbleGroup.h"
#include "World.h"
#include <map>
#include <vector>
#include <limits>
#include <GLFW/glfw3.h>
#include <random>

using namespace std;
using namespace glm;

AIBubbleGroup::AIBubbleGroup(vec3 position, float volume, glm::vec3 color) : BubbleGroup(position, volume, color)
{
	mClosestUnit = nullptr;
	mClosestThreat = nullptr;
	mClosestTarget = nullptr;
	nextPosition = World::GetInstance()->GetRandomPositionInsideUnitSphere();
}

AIBubbleGroup::~AIBubbleGroup()
{
}

void AIBubbleGroup::Update(float dt)
{
	setMoveTowards();
	BubbleGroup::Update(dt);
}

Bubble* AIBubbleGroup::getLargestBubble()
{
	float largestVolume = 0;
	Bubble* largestBubble;
	for (vector<Bubble*>::iterator it = this->GetBubbles()->begin(); it < this->GetBubbles()->end(); ++it)
	{
		// check for nullptr
		if (*it == nullptr)
			continue;

		if ((*it)->GetVolume() > largestVolume) {
			largestBubble = (*it);
		}
	}
	return largestBubble;
}

void AIBubbleGroup::setUnitBubbleDistances()
{
	World* mWorld = World::GetInstance();
	vector<Bubble*>* mBubbles = mWorld->GetBubbles();

	float minDistance = numeric_limits<float>::max();
	

	for (vector<Bubble*>::iterator it = mBubbles->begin(); it < mBubbles->end(); ++it)
	{
		// check for nullptr
		if (*it == nullptr)
			continue;

		// check min distance
		float distance = glm::distance((*it)->GetPosition(), GetCenterOfMass());
		if (distance < minDistance) {
			minDistance = distance;
			mClosestUnit = (*it);
			nextPosition = mClosestUnit->GetPosition();
		}

	}
}

void AIBubbleGroup::setBubbleGroupDistances()
{
	World* mWorld = World::GetInstance();
	vector<BubbleGroup*>* mBubbleGroups = mWorld->GetBubbleGroups();

	float minBubbleGroupDistance = numeric_limits<float>::max();
	float minThreatDistance = numeric_limits<float>::max();

	for (vector<BubbleGroup*>::iterator it = mBubbleGroups->begin(); it < mBubbleGroups->end(); ++it)
	{
		// check for this AI bubblegroup or dead bubblegroups
		if ((*it) == this || (*it)->GetGroupVolume() == 0 )
			continue;

		// check min distance
		float distance = glm::distance((*it)->GetCenterOfMass(), GetCenterOfMass());


		if (distance < minBubbleGroupDistance && GetGroupVolume() > (*it)->GetGroupVolume()) {
			// Target BubbleGroup
			minBubbleGroupDistance = distance;
			mClosestTarget = (*it);
			float unitDistance = glm::distance(GetCenterOfMass(), nextPosition);
			if (distance < unitDistance && unitDistance != 0)
				nextPosition = mClosestTarget->GetCenterOfMass();

		}
		else if (glm::distance((*it)->GetCenterOfMass(), GetCenterOfMass()) < minThreatDistance)
		{
			// Threat BubbleGroup
			minThreatDistance = distance;
			mClosestThreat = (*it);
		}

	}
	
}

void AIBubbleGroup::setMoveTowards()
{

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> scoreModifier(1, 10);
	//float scoreModifier = ((float)rand() / (RAND_MAX)) * 100;


	// find closest bubbles
	setUnitBubbleDistances();
	setBubbleGroupDistances();

	// set score
	float unitScore = (float)scoreModifier(rng);
	float unitDistance = 0;

	float targetScore = 0;
	float targetVolumeDifference = 0;
	float targetDistance = numeric_limits<float>::max();
	float threatDistance = 0;


	if (mClosestUnit != nullptr) {
		unitDistance = glm::distance(this->GetCenterOfMass(), mClosestUnit->GetPosition());
		unitScore += (1 / unitDistance);
	}

	if ((mClosestTarget != nullptr) && (mClosestTarget->GetGroupVolume() > 0)) {
		targetDistance = glm::distance(this->GetCenterOfMass(), mClosestTarget->GetCenterOfMass());
		threatDistance = glm::distance(this->GetCenterOfMass(), mClosestThreat->GetCenterOfMass());
		targetVolumeDifference = this->GetGroupVolume() / mClosestTarget->GetGroupVolume();
		targetScore = (20/targetDistance) + scoreModifier(rng);
	} 

	/*
	if ( (targetScore > unitScore) && targetVolumeDifference > 1.2) {
		nextPosition = mClosestTarget->GetCenterOfMass();
	}
	else {
		nextPosition = mClosestUnit->GetPosition();
	}
	*/
	// Note unit distance is set before target distance...
	

	glm::vec3 direction = (nextPosition - GetCenterOfMass());
	mMoveTowards = direction;
	/*
	if (this->shouldSplit(targetVolumeDifference, targetDistance)) {
		this->Split();
	}
	*/
}

bool AIBubbleGroup::shouldSplit(float volumeDifference, float distance)
{
	return ((volumeDifference > this->GetGroupVolume() * 2) && (distance < 20));
		
}

