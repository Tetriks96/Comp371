//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#include "Renderer.h"
#include "World.h"
#include "EventManager.h"

int main(int argc, char*argv[])
{
	EventManager::Initialize();
	Renderer::Initialize();

	do
	{
		World world;
		bool restartRequested = false;

		if (argc > 1)
		{
			world.LoadScene(argv[1]);
		}
		else
		{
			world.LoadScene("../Assets/Scenes/PrototypeUniverse.scene");
		}

		// Main Loop
		do
		{
			// Update Event Manager - Frame time / input / events processing 
			EventManager::Update();

			// Update World
			float dt = EventManager::GetFrameTime();
			world.Update(dt);

			// Draw World
			world.Draw();
		} while (EventManager::ExitRequested() == false && EventManager::RestartRequested() == false);

	} while (EventManager::ExitRequested() == false);

	Renderer::Shutdown();
	EventManager::Shutdown();

	return 0;
}
