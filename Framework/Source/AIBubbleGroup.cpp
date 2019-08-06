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
	vector<Bubble*> mBubbles = (*this->GetBubbles());
	float largestVolume = 0;
	Bubble* largestBubble;
	for (vector<Bubble*>::iterator it = mBubbles.begin(); it < mBubbles.end(); ++it)
	{
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
		// check for nullptr
		if ((*it) == this || (*it)->GetGroupVolume() == 0 )
			continue;

		// check min distance
		float distance = glm::distance((*it)->GetCenterOfMass(), GetCenterOfMass());


		if (distance < minBubbleGroupDistance && GetGroupVolume() > (*it)->GetGroupVolume()) {
			// Target BubbleGroup
			minBubbleGroupDistance = distance;
			mClosestTarget = (*it);
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


	// find closest unit & set unit Bubble score
	setUnitBubbleDistances();
	float unitScore = (float)scoreModifier(rng);
	if (mClosestUnit != nullptr) {
		float unitDistance = glm::distance(this->GetCenterOfMass(), mClosestUnit->GetPosition());
		unitScore += (1 / unitDistance);
	}

	// find closest BubbleGroup & set BubbleGroup score
	// * Note threat score not in use...
	setBubbleGroupDistances();
	float targetScore = 0;
	float targetVolumeDifference = 0;
	if ((mClosestTarget != nullptr) && (mClosestTarget->GetGroupVolume() > 0)) {
		float targetDistance = glm::distance(this->GetCenterOfMass(), mClosestTarget->GetCenterOfMass());
		float threatDistance = glm::distance(this->GetCenterOfMass(), mClosestThreat->GetCenterOfMass());
		targetVolumeDifference = this->GetGroupVolume() / mClosestTarget->GetGroupVolume();
		targetScore = (20/targetDistance) + scoreModifier(rng);
	} 

	glm::vec3 nextPosition;

	// * Note larger the score the better!

	if ( (targetScore > unitScore) && targetVolumeDifference > 1.2) {
		nextPosition = mClosestTarget->GetCenterOfMass();
	}
	else {
		nextPosition = mClosestUnit->GetPosition();
	}

	// reset Closest Bubble references
	mClosestUnit = nullptr;

	glm::vec3 direction = (nextPosition - GetCenterOfMass());
	mMoveTowards = direction;
}
