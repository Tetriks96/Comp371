#pragma once

#include <vector>
#include "SphereModel.h"

class Camera;
class ControllableSphere;

class WorldDrawer
{
public:
	static void WorldDrawer::DrawWorld(
		std::vector<Camera*> camera,
		unsigned int currentCamera,
		std::vector<SphereModel*> model,
		std::vector<ControllableSphere*> sphere);
};