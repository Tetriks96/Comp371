#pragma once

#include "Model.h"
#include "Animation.h"

class SceneLoader
{
public:
	static void LoadScene(
		const char * scene_path,
		std::vector<Model*>* model,
		std::vector<Animation*>* animation,
		std::vector<AnimationKey*>* animationKey
	);
};