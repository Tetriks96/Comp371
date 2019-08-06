#pragma once

#include "BubbleGroup.h"
#include "World.h"
#include <GLFW/glfw3.h>
#include <map>

class AIBubbleGroup : public BubbleGroup
{
public:
	AIBubbleGroup(glm::vec3 position, float volume, glm::vec3 color);
	~AIBubbleGroup();
	virtual void Update(float dt);

protected:
	void setLargestBubble();
	void setUnitBubbleDistances(); // returns the coordinates of the closest unit sphere
	void setBubbleGroupDistances();
	void setMoveTowards();
	void compareBubbleGroups(BubbleGroup* bubbleGroup);
	bool shouldSplit(float volumeDifference, float distance);

private:
	BubbleGroup* mClosestThreat;
	BubbleGroup* mClosestTarget; 
	Bubble* mClosestBubbleTarget;
	Bubble* mClosestUnit;
	Bubble* mLargestBubble;

	glm::vec3 nextPosition;

	// If we ever need to add the plane algorithm we will need these...
	enum Plane { top, bottom, left, right, front, back };

	std::map<BubbleGroup*, float>* bubbleGroupDistances; // distance between AI and all other bubble groups
	std::map<BubbleGroup*, float> bubbleGroupThreats; // threat level between AI and all other bubble groups
	std::map<BubbleGroup *, Plane> m;
	std::vector<float> unitDistances;
	std::vector<float> vBubbleGroupDistances;
	
};