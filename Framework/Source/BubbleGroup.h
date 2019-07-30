#pragma once

#include "SphereModel.h"
#include <vector>

class BubbleGroup
{
public:
	BubbleGroup(glm::vec3 centerOfMass, float volume);
	~BubbleGroup();
	virtual void Update(float dt);
	void Draw();
	glm::vec3 GetCenterOfMass() { return mCenterOfMass; }
	float GetVolume() { return mVolume; }
	float GetRadius() { return mRadius; }
protected:
	virtual void Split();
	std::vector<SphereModel*> mSphereModels;
	glm::vec3 mMoveTowards;
	float mVolume; // TODO: Make this private
	float mRadius; // TODO: Make this private
private:
	glm::vec3 mCenterOfMass;
};