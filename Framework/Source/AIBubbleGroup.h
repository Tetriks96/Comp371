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
	Bubble* getLargestBubble();
	void setUnitBubbleDistances(); // returns the coordinates of the closest unit sphere
	void setBubbleGroupThreats(); // returns the coordinates of the nearest bubble that is larger
	void setBubbleGroupDistances();
	void setMoveTowards();
	glm::vec3 getClosestBubblePosition();

private:
	enum Plane { top, bottom, left, right, front, back };
	//std::map<Bubble*, float>* unitDistances;
	std::vector<float> unitDistances;
	std::vector<float> vBubbleGroupDistances;
	glm::vec3 mClosestBubblePosition;
	std::map<BubbleGroup*, float>* bubbleGroupDistances; // distance between AI and all other bubble groups
	std::map<BubbleGroup*, float> bubbleGroupThreats; // threat level between AI and all other bubble groups
	std::map<BubbleGroup *, Plane> m;
};