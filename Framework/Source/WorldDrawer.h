#pragma once

#include <vector>

class Camera;
class Model;
class ControllableSphere;

class WorldDrawer
{
public:
	static void WorldDrawer::DrawWorld(
		std::vector<Camera*> camera,
		unsigned int currentCamera,
		std::vector<Model*> model,
		std::vector<ControllableSphere*> sphere);
};