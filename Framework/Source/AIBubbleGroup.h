#pragma once

#include "BubbleGroup.h"
#include <map>

class AIBubbleGroup : public BubbleGroup
{
public:
	virtual void Update(float dt);

protected:
	float getNearestUnitSphere(); // returns the coordinates of the closest unit sphere
	float getNearestThreat(); // returns the coordinates of the nearest bubble that is larger
	float getNearestTarget(); // returns the coordinates of the nearest bubble that is smaller
	void calculateNextPosition(); // find the best position to move to
	glm::vec3 getDirectionVector(BubbleGroup* enemy);
	

private:
	enum Plane { top, bottom, left, right, front, back };
	std::vector<int> vulnerableBubblesList; // vector of number of bubbles that are vulnerable for each enemy Bubble (should make map?)
	std::vector<int> threatLevelList; // threat level for each bubble group
	std::vector<int> targetBubbleList; // target bubble list
	std::map<BubbleGroup *, Plane> m;

};