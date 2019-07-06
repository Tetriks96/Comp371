#pragma once

#include <vector>

class Camera;
class Model;
class Animation;
class AnimationKey;
class ControllableSphere;

class WorldDrawer
{
public:
	static void WorldDrawer::DrawWorld(
		std::vector<Camera*> camera,
		unsigned int currentCamera,
		std::vector<Model*> model,
		std::vector<Animation*> animation,
		std::vector<AnimationKey*> animationKey,
		std::vector<ControllableSphere*> sphere);
};