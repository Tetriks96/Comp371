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
#include "ThirdPersonCamera.h"
#include "WorldDrawer.h"
#include "SceneLoader.h"
#include "EventManager.h"
#include "PlayerBubbleGroup.h"

#include <GLFW/glfw3.h>
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
	// Bubbles
	for (vector<Bubble*>::iterator it = mBubbles.begin(); it < mBubbles.end(); ++it)
	{
		delete *it;
	}

	mBubbles.clear();

	for (vector<BubbleGroup*>::iterator it = mBubbleGroups.begin(); it < mBubbleGroups.end(); ++it)
	{
		delete *it;
	}
	
	mBubbleGroups.clear();

	// Camera
	for (vector<Camera*>::iterator it = mCameras.begin(); it < mCameras.end(); ++it)
	{
		delete *it;
	}
	mCameras.clear();
}

World* World::GetInstance()
{
    return instance;
}

vector<Bubble*>* World::GetBubbles()
{
	return &mBubbles;
}

vector<BubbleGroup*>* World::GetBubbleGroups()
{
	return &mBubbleGroups;
}

void World::Update(float dt)
{
	for (vector<BubbleGroup*>::iterator it = mBubbleGroups.begin(); it < mBubbleGroups.end(); ++it)
	{
		(*it)->Update(dt);
	}

	// Remove null bubbles
	for (vector<Bubble*>::iterator it = mBubbles.begin(); it < mBubbles.end();)
	{
		if (*it == nullptr)
		{
			it = mBubbles.erase(it);
		}
		else
		{
			it++;
		}
	}

	// Update current Camera
	mCameras[mCurrentCamera]->Update(dt);
}

void World::Draw()
{
	WorldDrawer::DrawWorld(
		mCameras,
		mCurrentCamera,
		mBubbles,
		mBubbleGroups);
}

void World::LoadScene(const char * scene_path)
{
	SceneLoader::LoadScene(
		scene_path,
		this
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
		vec3 position = maxDistance * GetRandomPositionInsideUnitSphere();
		float volume = minSize + ((float)rand() / RAND_MAX) * (maxSize - minSize);
		float color1 = (float)rand() / RAND_MAX;
		float color2 = (float)rand() / RAND_MAX;
		float color3 = (float)rand() / RAND_MAX;
		Bubble* bubble = new Bubble(position, volume, vec3(color1, color2, color3));

		mBubbles.push_back(bubble);
	}

	PlayerBubbleGroup* playerBubbleGroup = new PlayerBubbleGroup(playerSize);
	
	mBubbleGroups.push_back(playerBubbleGroup);

	mCameras.push_back(new ThirdPersonCamera(playerBubbleGroup));
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

const Camera* World::GetCurrentCamera() const
{
     return mCameras[mCurrentCamera];
}
