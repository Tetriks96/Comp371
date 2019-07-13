#pragma once

#include "World.h"
#include "Model.h"
#include "Animation.h"

class SceneLoader
{
public:
	static void LoadScene(
		const char * scene_path,
		World* world,
		std::vector<Model*>* model,
		std::vector<Animation*>* animation,
		std::vector<AnimationKey*>* animationKey
	);
};