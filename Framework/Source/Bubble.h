#pragma once

#include "SphereModel.h"

class Bubble
{
public:
	Bubble(glm::vec3 position, float volume, glm::vec3 color);
	~Bubble();

	void Draw();
	virtual void Update(float dt, glm::vec3 moveTowards, glm::vec3 gravity, bool split);

	glm::vec3 GetPosition() { return mSphereModel->GetPosition(); }
	float GetRadius() { return mRadius; }
	float GetVolume() { return mVolume; }
	void SetVolume(float volume);
protected:
private:
	float mRadius;
	float mVolume;
	SphereModel* mSphereModel;
	float CalculateRadius(float volume);
	float CalculateEquilibriumSpeed();
	void HandleCollisions(std::vector<Bubble*>* bubbles);
};