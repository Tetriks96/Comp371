#pragma once

#include "Bubble.h"

class BubbleGroup
{
public:
	BubbleGroup(glm::vec3 centerOfMass, float volume);
	~BubbleGroup();

	virtual void Update(float dt);
	void Draw();

	std::vector<Bubble*>* GetBubbles() { return &mBubbles; }
	glm::vec3 GetCenterOfMass() { return mCenterOfMass; }
	float GetGroupRadius() { return mGroupRadius; }
	float GetGroupVolume() { return mGroupVolume; }
protected:
	virtual void Split();
	virtual void Pop();
	std::vector<Bubble*> mBubbles;
	glm::vec3 mMoveTowards;
private:
	glm::vec3 mCenterOfMass;
	float mGroupVolume;
	float mGroupRadius;
	double mLastSplitTime;

	glm::vec3 CalculateCenterOfMass();
	float CalculateGroupRadius();
	float CalculateGroupVolume();
};