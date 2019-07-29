#pragma once

#include "SphereModel.h"
#include <vector>

class BubbleGroup
{
public:
	virtual void Update(float dt) = 0;
	void Draw();
protected:
	std::vector<SphereModel*> mSphereModels;
private:
};