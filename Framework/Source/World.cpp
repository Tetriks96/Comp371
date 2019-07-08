//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#include "World.h"
#include "Renderer.h"
#include "ParsingHelper.h"

#include "StaticCamera.h"
#include "ThirdPersonCamera.h"

#include "CubeModel.h"
#include "SphereModel.h"
#include "ControllableSphere.h"
#include "Animation.h"
#include <GLFW/glfw3.h>
#include "EventManager.h"

#include "WorldDrawer.h"
#include "SceneLoader.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

World* World::instance;


World::World()
{
    instance = this;

	SphereModel* sphereModel = new SphereModel(vec3(1.01f), vec3(0.05f, 0.05f, 1.0f));
	ControllableSphere * controllableSphere = new ControllableSphere(sphereModel);
	mSphere.push_back(controllableSphere);

	// Setup Camera
	mCamera.push_back(new ThirdPersonCamera(controllableSphere));
	mCurrentCamera = 0;
}

World::~World()
{
	// Models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		delete *it;
	}

	mModel.clear();

	for (vector<ControllableSphere*>::iterator it = mSphere.begin(); it < mSphere.end(); ++it)
	{
		delete *it;
	}
	
	mSphere.clear();

	for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
	{
		delete *it;
	}

	mAnimation.clear();

	for (vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
	{
		delete *it;
	}

	mAnimationKey.clear();

	// Camera
	for (vector<Camera*>::iterator it = mCamera.begin(); it < mCamera.end(); ++it)
	{
		delete *it;
	}
	mCamera.clear();
}

World* World::GetInstance()
{
    return instance;
}

vector<Model*>* World::GetModels()
{
	return &mModel;
}

void World::Update(float dt)
{
	// Update animation and keys
    for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
    {
        (*it)->Update(dt);
    }
    
    for (vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
    {
        (*it)->Update(dt);
    }

	// Update current Camera
	mCamera[mCurrentCamera]->Update(dt);

	// Update models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		if (*it != nullptr)
		{
			(*it)->Update(dt);
		}
	}

	for (vector<ControllableSphere*>::iterator it = mSphere.begin(); it < mSphere.end(); ++it)
	{
		(*it)->Update(dt);
	}
}

void World::Draw()
{
	WorldDrawer::DrawWorld(
		mCamera,
		mCurrentCamera,
		mModel,
		mAnimation,
		mAnimationKey,
		mSphere);
}

void World::LoadScene(const char * scene_path)
{
	//SceneLoader::LoadScene(
	//	scene_path,
	//	&mModel,
	//	&mAnimation,
	//	&mAnimationKey
	//);
	int numberOfSpheres = 100;
	int minSize = 1;
	int maxSize = 1;
	float maxDistance = 25.0f;
	for (int i = 0; i < numberOfSpheres; i++)
	{
		float size = minSize + ((float)rand() / RAND_MAX) * (maxSize - minSize);
		float color1 = (float)rand() / RAND_MAX;
		float color2 = (float)rand() / RAND_MAX;
		float color3 = (float)rand() / RAND_MAX;
		SphereModel* sphere = new SphereModel(vec3(size), vec3(color1, color2, color3));

		sphere->SetPosition(maxDistance * GetRandomPositionInsideUnitSphere());

		mModel.push_back(sphere);
	}
}

vec3 World::GetRandomPositionInsideUnitSphere()
{
	float x;
	float y;
	float z;
	vec3 position;
	do
	{
		x = -1.0f + 2 * (float)rand() / RAND_MAX;
		y = -1.0f + 2 * (float)rand() / RAND_MAX;
		z = -1.0f + 2 * (float)rand() / RAND_MAX;

		position = vec3(x, y, z);
	} while (length(position) > 1.0f);
	return position;
}

Animation* World::FindAnimation(ci_string animName)
{
    for(std::vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
    {
        if((*it)->GetName() == animName)
        {
            return *it;
        }
    }
    return nullptr;
}

AnimationKey* World::FindAnimationKey(ci_string keyName)
{
    for(std::vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
    {
        if((*it)->GetName() == keyName)
        {
            return *it;
        }
    }
    return nullptr;
}

const Camera* World::GetCurrentCamera() const
{
     return mCamera[mCurrentCamera];
}
