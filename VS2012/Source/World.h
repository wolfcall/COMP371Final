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
class SheepModel; // Final project
class SheepParticleModel;// Final project

class World
{
public:
	World();
	~World();
    static World* GetInstance();

	void Update(float dt);
	void Draw();

	// To store tree positions

	//void treePosition();



	//World initial objects
	void init();

	//mesh changing
	void meshExplostion(); 
	Model* findMesh(std::string);

	void SheepSpawn(float dt);
	void treeSpawn(int num);
	void TransparentSheepSpawn();

	void LoadScene(const char * scene_path);
    void LoadCameras();

    Path* FindPath(ci_string pathName);
    BSpline* FindSpline(ci_string pathName);
    BSpline* FindSplineByIndex(unsigned int index);
    Model* FindModelByIndex(unsigned int index);

	template <typename T, typename... Args>
	Model *CreateModel(const char* name, Args... args)
	{
		auto res = new T(args...);
		mModel.push_back(res);
		mMeshes.push_back(new Meshes());
		mMeshes[mMeshes.size() - 1]->mesh = res;
		mMeshes[mMeshes.size() - 1]->name = const_cast<char*>(name);
		mMeshes[mMeshes.size() - 1]->strName = std::string(name);
		return res;
	}

	struct Meshes{
		char* name;
		std::string strName;
		Model* mesh;
	};
	void treeCollision(Model* mainObj);
	void boostCollision(Model* mainObj);
	void generateBoost();
private:
	int trees;
	int lives;
	bool hasDied;
    static World* instance;
	std::vector<Meshes*> mMeshes;
	std::vector<Model*> mModel;
    std::vector<Path*> mPath;
    std::vector<BSpline*> mSpline;
	std::vector<Camera*> mCamera;
	unsigned int mCurrentCamera;
	std::vector<SheepModel*> mSheep; // Final project: declaration of sheep array
	std::vector<SheepParticleModel*> mSheepParticle; // Final project: declaration of sheep particle array
	std::vector<Model *> mSacrifice;
	std::vector<Model *> mTrees;
};
