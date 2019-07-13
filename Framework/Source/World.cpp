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

	// Update current Camera
	mCamera[mCurrentCamera]->Update(dt);
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
	SceneLoader::LoadScene(
		scene_path,
		this,
		&mModel,
		&mAnimation,
		&mAnimationKey
	);
}

void World::ParseLine(
	ci_string line,
	int* numberOfSpheres,
	float* minSize,
	float* maxSize,
	float* maxDistance,
	float* playerSize,
	vec3* playerPosition,
	vec3* playerColor)
{
	// Splitting line into tokens
	ci_istringstream strstr(line);
	istream_iterator<ci_string, char, ci_char_traits> it(strstr);
	istream_iterator<ci_string, char, ci_char_traits> end;
	vector<ci_string> token(it, end);

	bool tokenParsedSuccessfully = false;

	if (token.size() == 0)
	{
		tokenParsedSuccessfully = true;
	}
	else if (token[0] == "name")
	{
		tokenParsedSuccessfully = true;
	}
	else if (token[0] == "numberOfSpheres")
	{
		assert(token.size() > 2);
		assert(token[1] == "=");

		*numberOfSpheres = static_cast<int>(atof(token[2].c_str()));

		tokenParsedSuccessfully = true;
	}
	else if (token[0] == "minSize")
	{
		assert(token.size() > 2);
		assert(token[1] == "=");

		*minSize = static_cast<float>(atof(token[2].c_str()));

		tokenParsedSuccessfully = true;
	}
	else if (token[0] == "maxSize")
	{
		assert(token.size() > 2);
		assert(token[1] == "=");

		*maxSize = static_cast<float>(atof(token[2].c_str()));

		tokenParsedSuccessfully = true;
	}
	else if (token[0] == "maxDistance")
	{
		assert(token.size() > 2);
		assert(token[1] == "=");

		*maxDistance = static_cast<float>(atof(token[2].c_str()));

		tokenParsedSuccessfully = true;
	}
	else if (token[0] == "playerSize")
	{
		assert(token.size() > 2);
		assert(token[1] == "=");

		*playerSize = static_cast<float>(atof(token[2].c_str()));

		tokenParsedSuccessfully = true;
	}
	else if (token[0] == "playerPosition")
	{
		assert(token.size() > 4);
		assert(token[1] == "=");

		playerPosition->x = static_cast<float>(atof(token[2].c_str()));
		playerPosition->y = static_cast<float>(atof(token[3].c_str()));
		playerPosition->z = static_cast<float>(atof(token[4].c_str()));

		tokenParsedSuccessfully = true;
	}
	else if (token[0] == "playerColor")
	{
		assert(token.size() > 4);
		assert(token[1] == "=");

		playerColor->x = static_cast<float>(atof(token[2].c_str()));
		playerColor->y = static_cast<float>(atof(token[3].c_str()));
		playerColor->z = static_cast<float>(atof(token[4].c_str()));

		tokenParsedSuccessfully = true;
	}

	if (tokenParsedSuccessfully == false)
	{
		fprintf(stderr, "Error loading scene file... token:  %s!", token[0].c_str());
		getchar();
		exit(-1);
	}
}

void World::Load(ci_istringstream& iss)
{

	int numberOfSpheres = 0;
	float minSize = 0.0f;
	float maxSize = 0.0f;
	float maxDistance = 0.0f;
	float playerSize = 0.0f;
	vec3 playerPosition = vec3(0.0f);
	vec3 playerColor = vec3(0.0f);

	ci_string line;

	// Parse world line by line
	while (std::getline(iss, line))
	{
		ParseLine(
			line,
			&numberOfSpheres,
			&minSize,
			&maxSize,
			&maxDistance,
			&playerSize,
			&playerPosition,
			&playerColor
		);
	}

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

	SphereModel* sphereM = new SphereModel(vec3(playerSize), playerColor);
	ControllableSphere* cSphere = new ControllableSphere(sphereM);

	mSphere.push_back(cSphere);

	mCamera.push_back(new ThirdPersonCamera(cSphere));
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
