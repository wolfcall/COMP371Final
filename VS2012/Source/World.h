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
class BSpline;

class World
{
public:
	World();
	~World();
    static World* GetInstance();

	void Update(float dt);
	void Draw();
	
	//World initial objects
	void init();

	//mesh changing
	void meshExplostion(); 

	void LoadScene(const char * scene_path);
    void LoadCameras();

    Path* FindPath(ci_string pathName);
    BSpline* FindSpline(ci_string pathName);
    BSpline* FindSplineByIndex(unsigned int index);
    Model* FindModelByIndex(unsigned int index);

	template <typename T, typename... Args>
	Model *CreateModel(std::string name, Args... args)
	{
		auto res = new T(args...);
		mModel.push_back(res);
		Meshes newMesh;
		newMesh.mesh = res;
		newMesh.name = name;
		if (mMeshes == NULL){
			mMeshes = new Meshes[1];
			mMeshes[0] = newMesh;
		}
		else{
			Meshes[] tMesh = mMeshes;
			mMeshes = new Meshes[(sizeof(mMeshes) / sizeof(*mMeshes)) + 1];
			for (int x = 0; x < (sizeof(mMeshes) / sizeof(*mMeshes)); x++){
				*(mMeshes + x) = *(tMesh + x);
			}
			mMeshes[(sizeof(mMeshes) / sizeof(*mMeshes)) - 1] = newMesh;
			free(tMesh);
		}
		return res;
	}

	struct Meshes{
		std::string name;
		Model* mesh;
	};

private:
    static World* instance;
	Meshes * mMeshes;
	std::vector<Model*> mModel;
    std::vector<Path*> mPath;
    std::vector<BSpline*> mSpline;
	std::vector<Camera*> mCamera;
	unsigned int mCurrentCamera;
};
