#include "AIBubbleGroup.h"
#include <map>
#include <vector>
#include <limits>
using namespace std;

void AIBubbleGroup::Update(float dt)
{

}

void AIBubbleGroup::setUnitBubbleDistances()
{
	float minDistance = std::numeric_limits<float>::max();

	vector<Bubble*> mBubbles = (*mWorld->GetBubbles());
	for (vector<Bubble*>::iterator it = mBubbles.begin(); it < mBubbles.end(); ++it)
	{
		float distance = glm::distance((*it)->GetPosition(), this->GetCenterOfMass());
		this->unitDistances.push_back(distance);
	}
}

void AIBubbleGroup::setBubbleGroupThreats()
{
	/*
		function loops through all bubbles of all bubblegroups
		calculates distances between AI and bubble groups.
		calculates threat level between AI and bubble groups
	*/
	float minDistance = std::numeric_limits<float>::max();
	vector<BubbleGroup*> mBubbleGroups = (*mWorld->GetBubbleGroups());

	// Loop through bubble groups
	for (vector<BubbleGroup*>::iterator it = mBubbleGroups.begin(); it < mBubbleGroups.end(); ++it)
	{
		// calculate distance
		float distance = glm::distance((*it)->GetCenterOfMass(), this->GetCenterOfMass());
		bubbleGroupDistances.insert(pair<BubbleGroup*, float>((*it), distance));

		// calculate vulnerabilities
		vector<Bubble*> mBubbles = (*it)->getBubbles(); // TODO implement getBubbles
		float vulnerability = 0;

		for (vector<Bubble*>::iterator it = mBubbles.begin(); it < mBubbles.end(); ++it)
		{
			Bubble* largest = this->getLargestBubble(); // TODO implement getLargestBubble
			float volumeDifference = this->largest->getVolume() - (*it)->GetVolume(); 
			vulnerability += volumeDifference;
		}

		bubbleGroupThreats.insert(pair<BubbleGroup*, float>((*it), vulnerability));


	}
}


