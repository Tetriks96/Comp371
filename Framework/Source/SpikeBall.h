#pragma once

#include "Model.h"
#include "SphereModel.h"
#include "ConeModel.h"
#include <GL\glew.h>

class SpikeBall : Model
{
public:
	SpikeBall(glm::vec3 position, float radius, glm::vec3 color);
	virtual ~SpikeBall(void);
	virtual void Draw();
	glm::vec3 GetPosition() { return mSphereModel->GetPosition(); }
	float GetRadius() { return (coneRatio + constant) * mSphereModel->GetScaling().x; }
private:
	SphereModel* mSphereModel;
	ConeModel* mConeModel;
	const float coneRatio = 0.5f;
	const float constant = 0.97f;
};