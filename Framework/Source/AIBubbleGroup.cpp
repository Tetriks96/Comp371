#include "AIBubbleGroup.h"
#include "World.h"
#include <map>
#include <vector>
#include <limits>
#include <GLFW/glfw3.h>
<<<<<<< HEAD
=======
#include <random>
#include <algorithm>

>>>>>>> master
using namespace std;
using namespace glm;

AIBubbleGroup::AIBubbleGroup(vec3 position, float volume, glm::vec3 color) : BubbleGroup(position, volume, color)
{
<<<<<<< HEAD
	mClosestBubblePosition = glm::vec3(1.f);
=======
	threatBubbles = new map<Bubble*, Bubble*>();
	unitBubbles = new map<Bubble*, Bubble*>();
	targetBubbles = new map<Bubble*, Bubble*>();
	mTarget = nullptr;
	mAttacker = nullptr;
>>>>>>> master
}

AIBubbleGroup::~AIBubbleGroup()
{
}

void AIBubbleGroup::Update(float dt)
{
	setMoveTowards();
	BubbleGroup::Update(dt);
<<<<<<< HEAD
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
=======
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
>>>>>>> master
		}
	}
	return largestBubble;
}

void AIBubbleGroup::setUnitBubbleDistances()
{
	World* mWorld = World::GetInstance();
	vector<Bubble*>* mBubbles = mWorld->GetBubbles();
<<<<<<< HEAD
	float minDistance = numeric_limits<float>::max();
	glm::vec3 closestPosition = glm::vec3(numeric_limits<float>::max());
	for (vector<Bubble*>::iterator it = mBubbles->begin(); it < mBubbles->end(); ++it)
	{
		if (*it == nullptr)
			continue;
		float distance = glm::distance((*it)->GetPosition(), GetCenterOfMass());
		if (distance < minDistance) {
			closestPosition = (*it)->GetPosition();
			minDistance = distance;
		}
		unitDistances.push_back(distance);
		//(*unitDistances).insert(pair<Bubble*, float>((*it), distance));
	}
	this->mClosestBubblePosition = closestPosition;
=======

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
>>>>>>> master
}

void AIBubbleGroup::setBubbleGroupDistances()
{
<<<<<<< HEAD
	World* mWorld = World::GetInstance();
	vector<BubbleGroup*>* mBubbleGroups = mWorld->GetBubbleGroups();
	float minDistance = glm::distance(GetCenterOfMass(), mClosestBubblePosition);
	//float minDistance = numeric_limits<float>::max();
	glm::vec3 closestPosition = mClosestBubblePosition;
	for (vector<BubbleGroup*>::iterator it = mBubbleGroups->begin(); it < mBubbleGroups->end(); ++it)
	{
		if ((*it) == this || (*it)->GetGroupVolume() == 0 )
			continue;
		float distance = glm::distance((*it)->GetCenterOfMass(), GetCenterOfMass());
		if (distance < minDistance && GetGroupVolume() > (*it)->GetGroupVolume()) {
			closestPosition = (*it)->GetCenterOfMass();
			minDistance = distance;
		}
		unitDistances.push_back(distance);
		//(*unitDistances).insert(pair<Bubble*, float>((*it), distance));
	}
	this->mClosestBubblePosition = closestPosition;
}


void AIBubbleGroup::setBubbleGroupThreats()
{
	/*
		function loops through all bubbles of all bubblegroups
		calculates distances between AI and bubble groups.
		calculates threat level between AI and bubble groups
	*/
	float minDistance = numeric_limits<float>::max();
	World* mWorld = World::GetInstance();
	vector<BubbleGroup*> mBubbleGroups = (*mWorld->GetBubbleGroups());

	// Loop through bubble groups
	for (vector<BubbleGroup*>::iterator it = mBubbleGroups.begin(); it < mBubbleGroups.end(); ++it)
	{
		// calculate distance
		float distance = glm::distance((*it)->GetCenterOfMass(), this->GetCenterOfMass());
		(*bubbleGroupDistances).insert(pair<BubbleGroup*, float>((*it), distance));

		// calculate vulnerabilities
		vector<Bubble*> mBubbles = (*(*it)->GetBubbles());
		float vulnerability = 0;

		for (vector<Bubble*>::iterator it = mBubbles.begin(); it < mBubbles.end(); ++it)
		{
			
			Bubble* largest = this->getLargestBubble();
			float volumeDifference = (largest)->GetVolume() - (*it)->GetVolume(); 
			vulnerability += volumeDifference;
			
		}

		bubbleGroupThreats.insert(pair<BubbleGroup*, float>((*it), vulnerability));

	}
}

void AIBubbleGroup::setMoveTowards()
{
	setUnitBubbleDistances();
	setBubbleGroupDistances();
	//getClosestBubblePosition();

	glm::vec3 direction = (mClosestBubblePosition - GetCenterOfMass());
	mMoveTowards = direction;
}

glm::vec3 AIBubbleGroup::getClosestBubblePosition()
{
	float minDistance = numeric_limits<float>::max();
	glm::vec3 closestPosition = glm::vec3(1.f);
	/*
	for (map<Bubble*, float>::iterator it = unitDistances->begin(); it != unitDistances->end(); ++it)
	{
		float distance = glm::distance(it->first->GetPosition(), GetCenterOfMass());
		
		if (distance < minDistance) {
			closestPosition = it->first->GetPosition();
		}
	}
	*/

	return closestPosition;
}

=======
	
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
	float unitScore = calculateScore(unitBubbles, 1);
	
	unitScore *= unitScoreWeight;
	targetScore *= targetScoreWeight;

	Bubble* mLargestBubble = this->findLargestBubble();

	// based on score select next bubble to capture..
	if (targetScore < unitScore) {
		mTarget = mClosestUnit;
		auto bubblePos = unitBubbles->find(mClosestUnit);
		if (bubblePos != unitBubbles->end())
			mAttacker = bubblePos->second;
	}
	else {
		// random splitting
		if (mLargestBubble != nullptr) {
			// set move towards
			mAttacker = mLargestBubble;
			if (mLargestBubble->GetVolume() > 30 && scoreModifier > 9) {
				this->Split();
			}
		}
	}

	glm::vec3 nextPosition = mTarget->GetPosition();

	glm::vec3 direction = (nextPosition - mAttacker->GetPosition());
	mMoveTowards = direction;

	float mTargetDistance = glm::distance(mTarget->GetPosition(), mAttacker->GetPosition());
	if (shouldSplit(mTarget, mTargetDistance) && targetScore > unitScore && mLargestBubble->GetVolume() > 25)
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

float AIBubbleGroup::calculateScore(map<Bubble*, Bubble*>* bubbleMap, bool isUnit)
{
	float bubbleScore = 0;
	// calculate target bubble scores
	for (map<Bubble*, Bubble*>::iterator it = bubbleMap->begin(); it != bubbleMap->end(); it++) {
		float heuristicScore = this->heuristic(it->first, it->second);
		float threatScore = this->getBubbleThreatScore(it->first, it->second);
		float score = heuristicScore - threatScore;
		if (bubbleScore < score) {
			bubbleScore = score;
			if (!isUnit)
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
>>>>>>> master

