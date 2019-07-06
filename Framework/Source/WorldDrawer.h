#pragma once

#include "Camera.h"
#include "Model.h"
#include "Animation.h"

class WorldDrawer
{
public:
	static void WorldDrawer::DrawWorld(
		std::vector<Camera*> camera,
		unsigned int currentCamera,
		std::vector<Model*> model,
		std::vector<Animation*> animation,
		std::vector<AnimationKey*> animationKey);
};