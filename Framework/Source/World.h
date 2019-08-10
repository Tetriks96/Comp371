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

#include "Model.h"
#include "Bubble.h"
#include "BubbleGroup.h"
#include "SpikeBall.h"
#include "Endgame.h"

class Camera;
class ControllableSphere;

class World
{
public:
	World(Endgame* endgame);
	~World();

	static bool sStartupScreen;
	
    static World* GetInstance();

	void Update(float dt);
	void Draw();

	void LoadScene(const char * scene_path);
	void Load(ci_istringstream& iss);
	void ParseLine(
		ci_string line,
		int* numberOfSpheres,
		float* minSize,
		float* maxSize,
		float* maxDistance,
		float* playerSize,
		glm::vec3* playerPosition,
		glm::vec3* playerColor);

    const Camera* GetCurrentCamera() const;
	std::vector<Bubble*>* GetBubbles();
	std::vector<SpikeBall*>* GetSpikeBalls();
	std::vector<BubbleGroup*>* GetBubbleGroups();

	static glm::vec3 GetRandomPositionInsideUnitSphere();
    
private:
    static World* instance;
	Endgame* mEndgame;
    
	std::vector<Bubble*>* mBubbles;
	std::vector<SpikeBall*> mSpikeBalls;
	std::vector<Camera*> mCameras;
	std::vector<ConeModel*> mCones;
	unsigned int mCurrentCamera;
	std::vector<BubbleGroup*> mBubbleGroups;
	glm::vec3 GetRandomColor();
};
