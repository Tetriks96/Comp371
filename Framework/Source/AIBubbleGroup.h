#pragma once

#include "BubbleGroup.h"
#include "World.h"
#include <GLFW/glfw3.h>
<<<<<<< HEAD
=======
#include <unordered_set>
>>>>>>> master
#include <map>

class AIBubbleGroup : public BubbleGroup
{
public:
	AIBubbleGroup(glm::vec3 position, float volume, glm::vec3 color);
	~AIBubbleGroup();
	virtual void Update(float dt);

<<<<<<< HEAD
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
=======
	Bubble* findLargestBubble();

protected:
	void setUnitBubbleDistances();
	void setBubbleGroupDistances();
	void setMoveTowards();
	void compareBubbleGroups(BubbleGroup* bubbleGroup);

	float heuristic(Bubble* enemyBubble, Bubble* mBubble);
	float getBubbleThreatScore(Bubble* enemyBubble, Bubble* mBubble);
	float calculateScore(std::map<Bubble*, Bubble*>* bubbleMap, bool);

	bool shouldSplit(Bubble* target, float distance);

private:

	Bubble* mAttacker;
	Bubble* mTarget;
	Bubble* mClosestUnit;

	// These weights are super sensitive, probably best to leave them alone
	float unitScoreWeight = 6;
	float targetScoreWeight = 3;
	float threatScoreWeight = 8; 

	// dont need threat bubbles
	std::map<Bubble*, Bubble*>* threatBubbles; // Bubbles from other groups that are a threat mapped to closest member bubble (enemy, mBubble)
	std::map<Bubble*, Bubble*>* targetBubbles; // Bubbles from other groups that are capturable mapped to closest member bubble (enemy, mBubble)
	std::map<Bubble*, Bubble*>* unitBubbles; // closest unit bubbles mapped to closest member bubble (enemy, mBubble)
>>>>>>> master
};