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
#include "Menu.h"
#include "Endgame.h"
#include <GLFW/glfw3.h>

int main(int argc, char*argv[])
{
	EventManager::Initialize();
	Renderer::Initialize();

	do
	{
		Menu menu;
		Endgame end;
		World world(&end);

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

			bool gameOver = end.getWin() || end.getLoss();
			bool gameInProgress = !(World::sStartupScreen || gameOver);

			if (!gameInProgress || !menu.isPaused())
			{
				float dt = EventManager::GetFrameTime();
				world.Update(dt);
			}

			if (gameInProgress)
			{
				if (menu.isPaused() && EventManager::PlayGame())
				{
					menu.Resume();
				}
				else if (!menu.isPaused() && EventManager::PauseGame())
				{
					menu.Pause();
				}
			}

			// Draw

			Renderer::BeginFrame();

			world.Draw();

			if ((gameInProgress && menu.isPaused()) || World::sStartupScreen)
			{
				menu.Draw();
			}
			else if (gameOver)
			{
				end.Draw();
			}

			Renderer::EndFrame();

		} while (EventManager::ExitRequested() == false && !((end.getLoss() || end.getWin() || World::sStartupScreen) && EventManager::PlayGame()));

		World::sStartupScreen = false;

	} while (EventManager::ExitRequested() == false);

	Renderer::Shutdown();
	EventManager::Shutdown();

	return 0;
}
