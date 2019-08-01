#pragma once

#include <vector>
#include "Bubble.h"
#include "BubbleGroup.h"

class Camera;

class WorldDrawer
{
public:
	static void WorldDrawer::DrawWorld(
		std::vector<Camera*> camera,
		unsigned int currentCamera,
		std::vector<Bubble*> bubbles,
		std::vector<BubbleGroup*> bubbleGroups);
};