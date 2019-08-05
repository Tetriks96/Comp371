#include "AIBubbleGroup.h"
#include "World.h"
#include <map>
#include <vector>
#include <limits>
#include <GLFW/glfw3.h>
using namespace std;

AIBubbleGroup::AIBubbleGroup(float volume, glm::vec3 color) : BubbleGroup(glm::vec3(1.0f), volume, color)
{
	mClosestBubble = nullptr;
}

AIBubbleGroup::~AIBubbleGroup()
{
}

void AIBubbleGroup::Update(float dt)
{
	setMoveTowards();
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
	float minDistance = numeric_limits<float>::max();
	World* mWorld = World::GetInstance();
	vector<Bubble*>* mBubbles = mWorld->GetBubbles();
	Bubble* closestBubble;

	for (vector<Bubble*>::iterator it = mBubbles->begin(); it < mBubbles->end(); ++it)
	{
		if (*it == nullptr)
			continue;
		float distance = glm::distance((*it)->GetPosition(), GetCenterOfMass());
		unitDistances.insert(pair<Bubble*, float>((*it), distance));
		closestBubble = (*it);
	}
	this->mClosestBubble = closestBubble;
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
		bubbleGroupDistances.insert(pair<BubbleGroup*, float>((*it), distance));

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
	/*
	for (map<Bubble*, float>::iterator it = this.begin(); it < this->bubbleGroupDistances.end(); ++it)
	{

	}
	*/
	setUnitBubbleDistances();

	glm::vec3 direction = (mClosestBubble->GetPosition() - GetCenterOfMass());
	mMoveTowards = direction;
}

Bubble* AIBubbleGroup::getClosestBubble()
{
	return this->mClosestBubble;
}


