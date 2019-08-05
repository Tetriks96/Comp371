#pragma once

#include "World.h"

class SceneLoader
{
public:
	static void LoadScene(
		const char * scene_path,
		World* world
	);
};