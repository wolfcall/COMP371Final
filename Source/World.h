//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include "ParsingHelper.h"
#include <vector>

class Camera;
class Model;
class Path;

class World
{
public:
	World();
	~World();
	
    static World* GetInstance();

	void Update(float dt);
	void Draw();

	void LoadScene(const char * scene_path);
    Path* FindPath(ci_string pathName);

private:
    static World* instance;

	std::vector<Model*> mModel;
    std::vector<Path*> mPath;
	std::vector<Camera*> mCamera;
	unsigned int mCurrentCamera;
};
