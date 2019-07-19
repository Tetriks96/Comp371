#pragma once

#include "World.h"
#include "SphereModel.h"

class SceneLoader
{
public:
	static void LoadScene(
		const char * scene_path,
		World* world,
		std::vector<SphereModel*>* model
	);
};