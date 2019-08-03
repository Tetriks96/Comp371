#pragma once

#include "SphereModel.h"

class Bubble
{
public:
	Bubble(glm::vec3 position, float volume, glm::vec3 color);
	~Bubble();

	void Draw();
	virtual void Update(float dt, glm::vec3 moveTowards, glm::vec3 gravity);
	Bubble* Split(float dt, glm::vec3 direction);

	glm::vec3 GetPosition() { return mSphereModel->GetPosition(); }
	float GetRadius() { return mRadius; }
	float GetVolume() { return mVolume; }
	void SetVolume(float volume);
protected:
private:
	Bubble(glm::vec3 position, float volume, glm::vec3 color, glm::vec3 divisionVelocity, Bubble* splitFrom);

	float mRadius;
	float mVolume;
	SphereModel* mSphereModel;
	glm::vec3 mDivisionVelocity;
	Bubble* mSplitFrom;
	double mLastSplitTime;

	float CalculateRadius(float volume);
	float CalculateEquilibriumSpeed();
	void HandleCollisions(std::vector<Bubble*>* bubbles, glm::vec3 previousPosition);
	void UpdatePosition(float dt, glm::vec3 moveTowards, glm::vec3 gravity);
};