#pragma once

#include "Bubble.h"
#include "ConeModel.h"
#include <GL\glew.h>

class SpikeBall
{
public:
	SpikeBall(glm::vec3 position, float volume, glm::vec3 color);
	virtual ~SpikeBall(void);
	virtual void Draw();
private:
	Bubble* mBubble;
	std::vector<ConeModel*> mCones;
};

