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
private:
	SphereModel* mSphereModel;
	ConeModel* mConeModel;
};

