#pragma once

#include "BubbleGroup.h"
#include "World.h"
#include <GLFW/glfw3.h>
#include <unordered_set>
#include <map>

class AIBubbleGroup : public BubbleGroup
{
public:
	AIBubbleGroup(glm::vec3 position, float volume, glm::vec3 color);
	~AIBubbleGroup();
	virtual void Update(float dt);

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
};