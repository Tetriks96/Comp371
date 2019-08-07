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
	void setUnitBubbleDistances(); // returns the coordinates of the closest unit sphere
	void setBubbleGroupDistances();
	void setMoveTowards();
	void compareBubbleGroups(BubbleGroup* bubbleGroup);
	bool shouldSplit(Bubble* target, float distance);

private:
	BubbleGroup* mClosestThreat;
	BubbleGroup* mClosestTarget; 
	Bubble* mClosestBubbleTarget;
	Bubble* mClosestUnit;

	glm::vec3 nextPosition;
	std::vector<Bubble*> threatBubbles; // Bubbles from other groups that are a threat
	std::vector<Bubble*> capturableBubbles; // Bubbles from other groups that are capturable
	
	
};