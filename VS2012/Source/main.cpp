//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "Renderer.h"
#include "World.h"
#include "EventManager.h"
//#include "AssetsManager.hpp"
//#include "Model_Classes/MeshModel.hpp"

int main(int argc, char*argv[])
{
	EventManager::Initialize();
	Renderer::Initialize();

	World world;

	if (argc > 1)
	{
		world.LoadScene(argv[1]);
	}
	else
	{
#if defined(PLATFORM_OSX)
        //world.LoadScene("Scenes/CoordinateSystem.scene");
        //world.LoadScene("Scenes/PathScene.scene");
        //world.LoadScene("Scenes/BSplineScene.scene");
#else
        //world.LoadScene("../Scenes/CoordinateSystem.scene");
        //world.LoadScene("../Scenes/PathScene.scene");
		world.LoadScene("../Scenes/BSplineScene.scene");
#endif

	}

	world.init();

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
	}
	while(EventManager::ExitRequested() == false);

	Renderer::Shutdown();
	EventManager::Shutdown();

	return 0;
}
