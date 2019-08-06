#include "AIBubbleGroup.h"
#include "World.h"
#include <map>
#include <vector>
#include <limits>
#include <GLFW/glfw3.h>
using namespace std;
using namespace glm;

AIBubbleGroup::AIBubbleGroup(vec3 position, float volume, glm::vec3 color) : BubbleGroup(position, volume, color)
{
	mClosestBubblePosition = glm::vec3(1.f);
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
}

void AIBubbleGroup::setBubbleGroupDistances()
{
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


