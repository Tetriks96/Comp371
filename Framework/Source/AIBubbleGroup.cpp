#include "AIBubbleGroup.h"
#include "World.h"
#include <map>
#include <vector>
#include <limits>
#include <GLFW/glfw3.h>
#include <random>
#include <algorithm>

using namespace std;
using namespace glm;

AIBubbleGroup::AIBubbleGroup(vec3 position, float volume, glm::vec3 color) : BubbleGroup(position, volume, color)
{
	threatBubbles = new map<Bubble*, Bubble*>();
	unitBubbles = new map<Bubble*, Bubble*>();
	targetBubbles = new map<Bubble*, Bubble*>();
	mTarget = nullptr;
	mAttacker = nullptr;
}

AIBubbleGroup::~AIBubbleGroup()
{
}

void AIBubbleGroup::Update(float dt)
{
	setMoveTowards();
	BubbleGroup::Update(dt);
	// Clear memory

	threatBubbles = new map<Bubble*, Bubble*>();
	unitBubbles = new map<Bubble*, Bubble*>();
	targetBubbles = new map<Bubble*, Bubble*>();
	
}

Bubble* AIBubbleGroup::findLargestBubble()
{
	float largestVolume = 0;
	Bubble* largestBubble = nullptr;
	for (vector<Bubble*>::iterator it = this->GetBubbles()->begin(); it < this->GetBubbles()->end(); ++it)
	{
		// check for nullptr
		if (*it == nullptr)
			continue;

		if ((*it)->GetVolume() > largestVolume) {
			largestVolume = (*it)->GetVolume();
			largestBubble = *it;
		}
	}
	return largestBubble;
}

void AIBubbleGroup::setUnitBubbleDistances()
{
	World* mWorld = World::GetInstance();
	vector<Bubble*>* mBubbles = mWorld->GetBubbles();

	for (vector<Bubble*>::iterator mIt = GetBubbles()->begin(); mIt < GetBubbles()->end(); ++mIt) {

		if (*mIt == nullptr)
			continue;

		float minDistance = numeric_limits<float>::max();

		for (vector<Bubble*>::iterator it = mBubbles->begin(); it < mBubbles->end(); ++it) {
			// check for nullptr
			if (*it == nullptr)
				continue;

			// check min distance
			glm::vec3 unitPosition = (*it)->GetPosition();
			float distance = glm::distance(unitPosition,(*mIt)->GetPosition());

			if (distance < minDistance) {
				minDistance = distance;
				mClosestUnit = (*it);
				unitBubbles->insert(pair<Bubble*, Bubble*>(*it, *mIt));
			}
		}
	}
}

void AIBubbleGroup::setBubbleGroupDistances()
{
	
	World* mWorld = World::GetInstance();
	vector<BubbleGroup*>* mBubbleGroups = mWorld->GetBubbleGroups();

	for (vector<BubbleGroup*>::iterator it = mBubbleGroups->begin(); it < mBubbleGroups->end(); ++it)
	{
		// check for this AI bubblegroup or dead bubblegroups
		if ((*it) == this || (*it)->GetGroupVolume() == 0 )
			continue;

		this->compareBubbleGroups((*it));
	}	
}

void AIBubbleGroup::setMoveTowards()
{
	// used for random number
	// random number being used to avoid equilibrium state
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> random(1, 10);

	// set closest bubbles
	setUnitBubbleDistances();
	setBubbleGroupDistances();

	// set scores
	float scoreModifier = (float)random(rng);
	float targetScore = calculateScore(targetBubbles, 0);
	float unitScore = calculateScore(unitBubbles, targetScore);

	// apply weights
	targetScore *= this->targetScoreWeight;
	unitScore *= this->unitScoreWeight;

	// based on score select next bubble to capture..
	if (targetScore < unitScore) {
		mTarget = mClosestUnit;
		float minDistance = numeric_limits<float>::max();
		for (map<Bubble*, Bubble*>::iterator it = unitBubbles->begin(); it != unitBubbles->end(); it++) {
			float distance = glm::distance(it->first->GetPosition(), it->second->GetPosition());
			if (distance < minDistance) {
				minDistance = distance;
				mAttacker = it->second;
			}
		}
	}
	else {
		// random splitting
		Bubble* mLargestBubble = this->findLargestBubble();
		if (mLargestBubble != nullptr) {
			// set move towards
			mAttacker = mLargestBubble;
			if (mLargestBubble->GetVolume() > 30 && scoreModifier > 9.5) {
				this->Split();
			}
		}
	}

	glm::vec3 nextPosition = mTarget->GetPosition();

	glm::vec3 direction = (nextPosition - mAttacker->GetPosition());
	mMoveTowards = direction;

	float mTargetDistance = glm::distance(mTarget->GetPosition(), mAttacker->GetPosition());
	if (shouldSplit(mTarget, mTargetDistance) && targetScore > unitScore)
		this->Split();
	
}

void AIBubbleGroup::compareBubbleGroups(BubbleGroup* bubbleGroup)
{

	for (vector<Bubble*>::iterator mIt = this->GetBubbles()->begin(); mIt < this->GetBubbles()->end(); mIt++) {
		if (*mIt == nullptr )
			continue;

		float minTargetDistance = numeric_limits<float>::max();
		float minThreatDistance = numeric_limits<float>::max();

		for (vector<Bubble*>::iterator eIt = bubbleGroup->GetBubbles()->begin(); eIt < bubbleGroup->GetBubbles()->end(); eIt++) {

			if (*eIt == nullptr)
				continue;

			float distance = glm::distance((*eIt)->GetPosition(), (*mIt)->GetPosition());

			if ((*mIt)->GetVolume() > (*eIt)->GetVolume()) {
				// bubble is capturable
				if (distance < minTargetDistance) {
					// enemy bubble is capturable and closer than existing one
					targetBubbles->insert(pair<Bubble*, Bubble*>(*eIt, *mIt));
					minTargetDistance = distance;
				}			
			} else {
				// These bubbles are threats! 
				if (distance < minThreatDistance) {
					threatBubbles->insert(pair<Bubble*, Bubble*>(*eIt, *mIt));
					minThreatDistance = distance;
				} 
			}
		}
	}
}

float AIBubbleGroup::getBubbleThreatScore(Bubble* targetBubble, Bubble* mBubble) {
	
	World* mWorld = World::GetInstance();
	vector<BubbleGroup*>* mBubbleGroups = mWorld->GetBubbleGroups();

	float safeDistance = 10;
	float threatScore = 0;

	for (vector<BubbleGroup*>::iterator bgIt = mBubbleGroups->begin(); bgIt < mBubbleGroups->end(); ++bgIt)
	{
		if (*bgIt == nullptr)
			continue;

		for (vector<Bubble*>::iterator it = (*bgIt)->GetBubbles()->begin(); it < (*bgIt)->GetBubbles()->end(); ++it)
		{
			if (*it == nullptr)
				continue;

			float distance = glm::distance((*it)->GetPosition(), targetBubble->GetPosition());
			if ((*it)->GetVolume() > mBubble->GetVolume() && distance < safeDistance) {
				threatScore += distance;
				// come up with equation for distance to danger ratio
			}
		}
	}
	
	return threatScore * this->threatScoreWeight;
}

float AIBubbleGroup::calculateScore(map<Bubble*, Bubble*>* bubbleMap, float bubbleScore)
{
	// calculate target bubble scores
	for (map<Bubble*, Bubble*>::iterator it = bubbleMap->begin(); it != bubbleMap->end(); it++) {
		float heuristicScore = this->heuristic(it->first, it->second);
		float threatScore = this->getBubbleThreatScore(it->first, it->second);
		float score = heuristicScore - threatScore;
		if (bubbleScore < score) {
			bubbleScore = score;
			mTarget = it->first;
			mAttacker = it->second;
		}
	}
	return bubbleScore;
}

float AIBubbleGroup::heuristic(Bubble* enemyBubble, Bubble* mBubble)
{
	float score = 0;
	score += getBubbleThreatScore(enemyBubble, mBubble);

	float distance = glm::distance(enemyBubble->GetPosition(), mBubble->GetPosition());
	float volumeDifference = enemyBubble->GetVolume() - mBubble->GetVolume();

	// Create equation based on distance and volume difference..
	score += (enemyBubble->GetVolume())/distance; // smaller distnace the better, the larger the volume the better
	return score;
}

bool AIBubbleGroup::shouldSplit(Bubble* target, float distance)
{
	// change largest bubble to "Attacker Bubble"
	return ((target->GetVolume()*2.2 < mAttacker->GetVolume()) && (distance < 4));
}

