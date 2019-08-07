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
	mClosestBubbleTarget = nullptr;
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

void AIBubbleGroup::setUnitBubbleDistances()
{
	World* mWorld = World::GetInstance();
	vector<Bubble*>* mBubbles = mWorld->GetBubbles();
	mClosestUnit = nullptr;
	float minDistance = numeric_limits<float>::max();
	

	for (vector<Bubble*>::iterator it = mBubbles->begin(); it < mBubbles->end(); ++it)
	{
		// check for nullptr
		if (*it == nullptr)
			continue;

		// check min distance
		glm::vec3 unitPosition = (*it)->GetPosition();
		float distance = glm::distance(unitPosition, GetCenterOfMass());

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


	float minThreatDistance = numeric_limits<float>::max();
	float minTargetDistance = numeric_limits<float>::max();

	for (vector<BubbleGroup*>::iterator it = mBubbleGroups->begin(); it < mBubbleGroups->end(); ++it)
	{
		// check for this AI bubblegroup or dead bubblegroups
		if ((*it) == this || (*it)->GetGroupVolume() == 0 )
			continue;

		(*it)->setLargestBubble();

		// check min distance
		float distance = glm::distance((*it)->GetCenterOfMass(), GetCenterOfMass());

		if (distance < minTargetDistance && GetGroupVolume() > (*it)->GetGroupVolume()) {
			// Target BubbleGroup
			minTargetDistance = distance;
			mClosestTarget = (*it);
			compareBubbleGroups((*it));
			
		}
		else if (glm::distance((*it)->GetCenterOfMass(), GetCenterOfMass()) < minThreatDistance)
		{
			// Threat BubbleGroup
			minThreatDistance = distance;
			mClosestThreat = (*it);
			compareBubbleGroups((*it));
		}

	}
	
}

void AIBubbleGroup::setMoveTowards()
{
	// NOTE SCORES NOT BEING USED YET! 

	// used for random number
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> scoreModifier(1, 10);

	// set closest bubbles
	setLargestBubble();
	setUnitBubbleDistances();
	setBubbleGroupDistances();


	// set score
	float unitScore = (float)scoreModifier(rng);
	float unitDistance = 0;

	float targetScore = 0;
	float targetDistance = numeric_limits<float>::max();

	nextPosition = glm::vec3(numeric_limits<float>::max());

	if (mClosestUnit != nullptr) {
		unitDistance = glm::distance(this->GetCenterOfMass(), mClosestUnit->GetPosition());
		unitScore += (1 / unitDistance);
		nextPosition = mClosestUnit->GetPosition();
	}

	if ((mClosestTarget != nullptr) && (mClosestTarget->GetGroupVolume() > 0)) {
		targetDistance = glm::distance(this->GetCenterOfMass(), mClosestTarget->GetCenterOfMass());
		targetScore = (20/targetDistance) + scoreModifier(rng);

		float nextDistance = glm::distance(nextPosition, GetCenterOfMass());
		float mClosestTargetDistance = glm::distance(mClosestTarget->GetCenterOfMass(), GetCenterOfMass());

		if (mClosestTargetDistance < 1.3*nextDistance) {
			// Check if there is a bubble in BubbleGroup that you can eat
			if (mClosestBubbleTarget != nullptr) {
				nextPosition = mClosestBubbleTarget->GetPosition();

				if (mClosestTargetDistance < 2 && mLargestBubble->GetVolume() > 2 * mClosestBubbleTarget->GetVolume()) {
					this->Split();
				}
			}
			
		}
	} 

	glm::vec3 direction = (nextPosition - mLargestBubble->GetPosition());
	mMoveTowards = direction;
	float groupSize = this->GetBubbles()->size();

	if (mLargestBubble->GetVolume() > 30 && groupSize < 5 && scoreModifier(rng) > 9) {
		this->Split();
	}
	
}

void AIBubbleGroup::compareBubbleGroups(BubbleGroup* bubbleGroup)
{
	float minTargetDistance = numeric_limits<float>::max();


	for (vector<Bubble*>::iterator it = bubbleGroup->GetBubbles()->begin(); it < bubbleGroup->GetBubbles()->end(); it++) {
		
		if (*it == nullptr || mLargestBubble == nullptr)
			continue;

		if (mLargestBubble->GetVolume() > (*it)->GetVolume()) {

			float distance = glm::distance(mLargestBubble->GetPosition(), (*it)->GetPosition());

			if (distance < minTargetDistance) {
				minTargetDistance = distance;
				mClosestBubbleTarget = (*it);
			}

		}
	}
	
}

bool AIBubbleGroup::shouldSplit(float volumeDifference, float distance)
{

	return ((volumeDifference > this->GetGroupVolume() * 2) && (distance < 20));
		
}

