#pragma once

#include "SphereModel.h"

class Bubble
{
public:
	Bubble(glm::vec3 position, float volume, glm::vec3 color);
	~Bubble();

	void Draw();

	glm::vec3 GetPosition() { return mSphereModel->GetPosition(); }
	void SetPosition(glm::vec3 position) { mSphereModel->SetPosition(position); }

	float GetRadius() { return mRadius; }
	float GetVolume() { return mVolume; }
	void SetVolume(float volume);
protected:
	float mVolume;
	float mRadius;
	SphereModel* mSphereModel;
private:
	float CalculateRadius(float volume);
};