#include "AIBubbleGroup.h"
#include <map>
#include <limits>
using namespace std;

void AIBubbleGroup::Update(float dt)
{

}

float AIBubbleGroup::getNearestUnitSphere()
{
	float minDistance = std::numeric_limits<float>::max();
	for (int i = 0; i < unitList.size(); i++) {
		float distance = glm::distance(this.getCenterOfMass() < unitList[i].getCenterOfMass());
		if ( distance < minDistance) {
			minDistance = distance;
		}
	}
	return minDistance;
}

float AIBubbleGroup::getNearestThreat()
{
	float minDistance = std::numeric_limits<float>::max();
	vector<int> threatLevel;
	vector<int> distances;
	vector<int> vulnerableBubblesList;

	// loop through all bubble groups
	for (int i = 0; i < world.getBubbleGroups().size(); i++) {
		float distance = glm::distance(this.getCenterOfMass() < unitList[i].getCenterOfMass());

		// loop through all bubbles in bubblegroup
		for (SphereModel& enemyBubble : world.getBubbleGroups()) {
			// check to see if bubblegroup has a bubble that is larger than any bubble in AI bubble group
			int vulnerableBubbles = 0;
			for (SphereModel& aiBubble : this->mSphereModels) {
				if (aiBubble.radius < enemyBubble.radius) {
					vulnerableBubbles++;
					vulnerableBubblesList.push_back(vulnerableBubbles);
				}
				else {
					// fill target list...
				}
			}
			distances.push_back(distance(enemyBubble.centerOfMass, this.getCenterOfMass()))
		}
	}
	return minDistance;
}

float AIBubbleGroup::getNearestTarget()
{
	return 0.0f;
}



void AIBubbleGroup::calculateNextPosition()
{
	int distanceScore = 0;
	int targetScore = 0;
	int vulnerableScore = 0;
	// loop through vulnerable bubbles list, target list and distances calculate score for each
	

}

