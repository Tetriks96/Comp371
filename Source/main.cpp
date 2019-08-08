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
#include <GLFW/glfw3.h>

int main(int argc, char*argv[])
{
	EventManager::Initialize();
	Renderer::Initialize();

	Menu menu;
	World world;    
    
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

		Renderer::BeginFrame();
		if (menu.isPaused()) {
			world.Draw();
			menu.Draw();
			if (EventManager::PlayGame()) {
				menu.toggle();
			}
		}
		else {
			if (EventManager::PauseGame()) {
				menu.Draw();
				menu.toggle();
			}
			world.Update(dt);
			// Draw World
			world.Draw();
		}
		Renderer::EndFrame();
	}
	while(EventManager::ExitRequested() == false);

	Renderer::Shutdown();
	EventManager::Shutdown();

	return 0;
}
