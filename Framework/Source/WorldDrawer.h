#pragma once

#include <vector>
#include "SphereModel.h"
#include "BubbleGroup.h"
#include "ConeModel.h"

class Camera;

class WorldDrawer
{
public:
	static void WorldDrawer::DrawWorld(
		std::vector<Camera*> camera,
		unsigned int currentCamera,
		std::vector<SphereModel*> model,
		std::vector<BubbleGroup*> bubbleGroups);
};