#pragma once

#include <vector>
#include "Bubble.h"
#include "BubbleGroup.h"
#include "SpikeBall.h"

class Camera;

class WorldDrawer
{
public:
	static void WorldDrawer::DrawWorld(
		std::vector<Camera*> camera,
		unsigned int currentCamera,
		std::vector<Bubble*> bubbles,
		std::vector<SpikeBall*> spikeBalls,
		std::vector<BubbleGroup*> bubbleGroups);
};