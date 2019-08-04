#pragma once

#include "BubbleGroup.h"
#include "World.h"
#include <map>

class AIBubbleGroup : public BubbleGroup
{
public:
	virtual void Update(float dt);

protected:
	void setUnitBubbleDistances(); // returns the coordinates of the closest unit sphere
	void setBubbleGroupThreats(); // returns the coordinates of the nearest bubble that is larger
	

private:
	enum Plane { top, bottom, left, right, front, back };
	std::vector<float> unitDistances;
	std::map<BubbleGroup*, float> bubbleGroupDistances; // distance between AI and all other bubble groups
	std::map<BubbleGroup*, float> bubbleGroupThreats; // threat level between AI and all other bubble groups
	std::map<BubbleGroup *, Plane> m;
	World* mWorld;
};