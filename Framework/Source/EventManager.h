//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#pragma once

struct GLFWwindow;

class EventManager
{
public:
	static void Initialize();
	static void Shutdown();

	static void Update();

	static double GetGameTime() { return sGameTime; }
	static float GetFrameTime();
	static bool ExitRequested();
	static bool PlayGame();
	static bool PauseGame();

	static GLFWwindow* GetWindow();

	static float  GetMouseMotionX();
	static float  GetMouseMotionY();
	static double GetMouseMotionScroll();

	static void EnableMouseCursor();
	static void DisableMouseCursor();
    static float GetRandomFloat(float min, float max);

private:
	// Time
	static double sGameTime;
	static float sFrameTime;

	// Mouse
	static double sLastMousePositionX;
	static float  sMouseDeltaX;
	static double sLastMousePositionY;
	static float  sMouseDeltaY;
	static double sMouseDeltaScroll;

	// Window
	static GLFWwindow* spWindow;

	static void ScrollCallback(GLFWwindow* window, double x, double y);

	static bool sPlayGame;
	static bool sPauseGame;
};
