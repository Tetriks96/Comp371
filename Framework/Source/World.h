//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#pragma once


#include "ParsingHelper.h"
#include <vector>
#include "glm/fwd.hpp"

class Camera;
class Model;
class Animation;
class AnimationKey;
class ControllableSphere;

class World
{
public:
	World();
	~World();
	
    static World* GetInstance();

	void Update(float dt);
	void Draw();

	void LoadScene(const char * scene_path);
    Animation* FindAnimation(ci_string animName);
    AnimationKey* FindAnimationKey(ci_string keyName);

    const Camera* GetCurrentCamera() const;
	std::vector<Model*>* GetModels();
    
private:
    static World* instance;
    
	std::vector<Model*> mModel;
    std::vector<Animation*> mAnimation;
    std::vector<AnimationKey*> mAnimationKey;
	std::vector<Camera*> mCamera;
	unsigned int mCurrentCamera;
	std::vector<ControllableSphere*> mSphere;

	glm::vec3 GetRandomPositionInsideUnitSphere();
};
