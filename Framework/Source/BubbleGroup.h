#pragma once

#include "Bubble.h"
#include <vector>

class BubbleGroup
{
public:
	BubbleGroup(glm::vec3 centerOfMass, float volume);
	~BubbleGroup();

	virtual void Update(float dt);
	void Draw();

	glm::vec3 GetCenterOfMass() { return mCenterOfMass; }
	float GetGroupRadius() { return mGroupRadius; }
	float GetGroupVolume() { return mGroupVolume; }
protected:
	virtual void Split();
	std::vector<Bubble*> mBubbles;
	glm::vec3 mMoveTowards;
private:
	glm::vec3 mCenterOfMass;
	float mGroupVolume;
	float mGroupRadius;
};