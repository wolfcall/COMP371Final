//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "World.h"
#include "Renderer.h"
#include "ParsingHelper.h"

#include "Camera/StaticCamera.h"
#include "BSpline/BSplineCamera.h"
#include "Camera/ThirdPersonCamera.h"

#include "Model_Classes/CubeModel.h"
#include "Model_Classes/SphereModel.h"
#include "Model_Classes/SheepModel.h"
#include "Model_Classes/SheepParticleModel.h"
#include "Path.h"
#include "BSpline/BSpline.h"

#include <GLFW/glfw3.h>
#include "EventManager.h"

#include <windows.h>
#include "mmsystem.h"

#define SND_FILENAME 0x20000
#define SND_LOOP 8
#define SND_ASYNC 1

#include "Model_Classes/WormModel.h"
#include "AssetsManager.hpp"
#include "Model_Classes\MeshModel.hpp"

//For random number generator
#include <ctime>


using namespace std;
using namespace glm;

// Light Coefficients
const vec3 lightColor(1.0f, 1.0f, 1.0f);
const float lightKc = 0.0f;
const float lightKl = 0.0f;
const float lightKq = 2.0f;
const vec4 lightPosition(0.0f, 10.0f, 0.0f, 1.0f);

World* World::instance;
auto assetsManager = AssetsManager::getInstance();

World::World()
{
    instance = this;
}

World::~World()
{
	// Models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		delete *it;
	}

	mModel.clear();

    // Paths
    for (vector<Path*>::iterator it = mPath.begin(); it < mPath.end(); ++it)
	{
		delete *it;
	}
	mPath.clear();

    // Splines
    for (vector<BSpline*>::iterator it = mSpline.begin(); it < mSpline.end(); ++it)
	{
		delete *it;
	}
	mSpline.clear();

	// Camera
	for (vector<Camera*>::iterator it = mCamera.begin(); it < mCamera.end(); ++it)
	{
		delete *it;
	}
	mCamera.clear();
}

World* World::GetInstance()
{
    return instance;
}

void World::Update(float dt)
{
	// User Inputs
	// 0 1 2 to change the Camera
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_1 ) == GLFW_PRESS)
	{
		mCurrentCamera = 0;
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_2 ) == GLFW_PRESS)
	{
		if (mCamera.size() > 1)
		{
			mCurrentCamera = 1;
		}
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_3 ) == GLFW_PRESS)
	{
		if (mCamera.size() > 2)
		{
			mCurrentCamera = 2;
		}
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_4 ) == GLFW_PRESS)
	{
        // Spline camera
		if (mCamera.size() > 3 && mSpline.size() > 0)
		{
			mCurrentCamera = 3;
		}
	}
	/*
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_5 ) == GLFW_PRESS)
	{
        // Spline camera
		if (mCamera.size() > 4 && mModel.size() > 0)
		{
			mCurrentCamera = 4;
		}
	}
	*/
	// Spacebar to change the shader
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_0 ) == GLFW_PRESS)
	{
		Renderer::SetShader(GOURAUD_SHADER);
	}
	//else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_9 ) == GLFW_PRESS)
	//{
		//Renderer::SetShader(PHONG_SHADER);
	//}

	// Update current Camera
	mCamera[mCurrentCamera]->Update(dt);

	// Update models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		(*it)->Update(dt);
	}


	// ----------------------------------------------------------------------------------------------------------------------------------
	// Ning's Sheep
	// Final project
	for (vector<SheepModel*>::iterator it = mSheep.begin(); it < mSheep.end(); ++it) // Here vector = array 
	{
		(*it)->Update(dt);
	}

	// Ning's sheep particle
	// Final project
	for (vector<SheepParticleModel*>::iterator it = mSheepParticle.begin(); it < mSheepParticle.end(); ++it) // Here vector = array 
	{
		(*it)->SetViewMatrix(mCamera[mCurrentCamera]->GetViewMatrix());
		(*it)->Update(dt);
	}

	while (mSheepParticle.size()>0 && (mSheepParticle[0]->IsAlive() == false)){
		mSheepParticle.erase(mSheepParticle.begin()); // remove mSheepParticle[0] if it's dead
	}

	// Ning's collision
	//if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_P) == GLFW_PRESS)
	if (length(findMesh("Worm")->GetPosition() - mSheep[0]->GetPosition()) < 4.0f )
	{
		SheepParticleModel* character_sheep_particle = new SheepParticleModel(); // Final project	
		character_sheep_particle->SetPosition(mSheep[0]->GetPosition() + vec3(0.0f, 1.5f, 0.0f));	
		mSheepParticle.push_back(character_sheep_particle);											
		PlaySound(TEXT("../Sound/SHEEPBAA.WAV"), NULL, SND_FILENAME | SND_ASYNC);
		mSheep[0]->ResetPosition();

		// Final project
	}
	//watch
	//printf("number of sheep:%d\n", mSheep.size());
	// ----------------------------------------------------------------------------------------------------------------------------------



	//meshExplostion();
}

void World::Draw()
{
	Renderer::BeginFrame();

	glCullFace(GL_BACK);// default
	glEnable(GL_CULL_FACE);// hide all CW faces
	
	// Set shader to use
	// Get a handle for our Transformation Matrices uniform
	GLuint WorldMatrixID = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	GLuint ViewMatrixID = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewTransform");
	GLuint ProjMatrixID = glGetUniformLocation(Renderer::GetShaderProgramID(), "ProjectonTransform");

	// Get a handle for Light Attributes uniform
	GLuint LightPositionID = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldLightPosition");
	GLuint LightColorID = glGetUniformLocation(Renderer::GetShaderProgramID(), "lightColor");
	GLuint LightAttenuationID = glGetUniformLocation(Renderer::GetShaderProgramID(), "lightAttenuation");


	glUseProgram(Renderer::GetShaderProgramID());

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform"); 

	// Send the view projection constants to the shader
	mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	glm::mat4 worldMatrix(1.0f);
	glm::mat4 viewMatrix = mCamera[mCurrentCamera]->GetViewMatrix();
	glm::mat4 projectionMatrix = mCamera[mCurrentCamera]->GetProjectionMatrix();

	glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &worldMatrix[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &projectionMatrix[0][0]);

	// Shader constants for Light
	glUniform4f(LightPositionID, lightPosition.x, lightPosition.y, lightPosition.z, lightPosition.w);
	glUniform3f(LightColorID, lightColor.r, lightColor.g, lightColor.b);
	glUniform3f(LightAttenuationID, lightKc, lightKl, lightKq);

	// Draw models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		// Draw model
		(*it)->Draw();
	}

	// Draw Path Lines
	
	// Set Shader for path lines
	unsigned int prevShader = Renderer::GetCurrentShader();
	//Renderer::SetShader(SHADER_PATH_LINES);
	glUseProgram(Renderer::GetShaderProgramID());

	// Send the view projection constants to the shader
	VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	for (vector<Path*>::iterator it = mPath.begin(); it < mPath.end(); ++it)
	{
		// Draw model
		(*it)->Draw();
	}

    // Draw B-Spline Lines (using the same shader for Path Lines)
    for (vector<BSpline*>::iterator it = mSpline.begin(); it < mSpline.end(); ++it)
	{
		// Draw model
		//(*it)->Draw();
	}

	// Restore previous shader
	Renderer::SetShader((ShaderType) prevShader);


	//Ning's Sheep
	// ----------------------------------------------------------------------------------------------------------------------------------
		// Final project: Draw Sheep
		// Set Shader for Sheep
	prevShader = Renderer::GetCurrentShader();
	Renderer::SetShader(SHADER_SHEEP);
	glUseProgram(Renderer::GetShaderProgramID());

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint ViewMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewTransform");
	GLuint ProjectionMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ProjectonTransform");

	// Send the view projection constants to the shader
	mat4 ViewMatrix = mCamera[mCurrentCamera]->GetViewMatrix();
	mat4 ProjectionMatrix = mCamera[mCurrentCamera]->GetProjectionMatrix();
	glUniformMatrix4fv(ViewMatrixLocation, 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(ProjectionMatrixLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);

	for (vector<SheepModel*>::iterator it = mSheep.begin(); it < mSheep.end(); ++it)
	{
		// Draw model
		(*it)->Draw();
		//watch
		// printf("draw sheep:%f\n", EventManager::GetFrameTime());
		// printf("number of sheep particle:%u\n", mSheepParticle.size());
	}

	// Restore previous shader
	Renderer::SetShader((ShaderType)prevShader);
	// Final project Draw Sheep ends

	// ----------------------------------------------------------------------------------------------------------------------------------


	// Ning's sheep particle
	// ----------------------------------------------------------------------------------------------------------------------------------
	// Final project: Sacrifice for particle draw
	mSacrifice[0]->Draw();

	// Final project: Draw Sheep Particle
	// Set Shader for Sheep Particle
	prevShader = Renderer::GetCurrentShader();
	Renderer::SetShader(SHADER_SHEEP_PARTICLE);
	glUseProgram(Renderer::GetShaderProgramID());

	GLuint programID = Renderer::GetShaderProgramID();
	// Send the view projection constants to the shader
	// Each frame has only 1 camera, so everything regarding to camera is put into World.cpp
	GLuint CameraRight_worldspace_ID = glGetUniformLocation(programID, "CameraRight_worldspace");
	GLuint CameraUp_worldspace_ID = glGetUniformLocation(programID, "CameraUp_worldspace");
	GLuint ViewProjMatrixID = glGetUniformLocation(programID, "VP");

	// Send the view projection constants to the shader
	glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]); // [column][row]
	glUniform3f(CameraUp_worldspace_ID, ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);	// [column][row]
	glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &VP[0][0]);

	for (vector<SheepParticleModel*>::iterator it = mSheepParticle.begin(); it < mSheepParticle.end(); ++it)
	{
		// Draw model
		(*it)->Draw();
	}

	// Restore previous shader
	Renderer::SetShader((ShaderType)prevShader);
	// Final project Draw Sheep Particle ends


	// ----------------------------------------------------------------------------------------------------------------------------------


	Renderer::EndFrame();
}

void World::LoadScene(const char * scene_path)
{
	// Using case-insensitive strings and streams for easier parsing
	ci_ifstream input;
	input.open(scene_path, ios::in);

	// Invalid file
	if(input.fail() )
	{	 
		fprintf(stderr, "Error loading file: %s\n", scene_path);
		getchar();
		exit(-1);
	}

	ci_string item;
	while( std::getline( input, item, '[' ) )   
	{
        ci_istringstream iss( item );

		ci_string result;
		if( std::getline( iss, result, ']') )
		{

			if( result == "cube" )
			{
				// Box attributes
				CubeModel* cube = new CubeModel();
				cube->Load(iss);
				//mModel.push_back(cube);
			}
            else if( result == "sphere" )
            {
                SphereModel* sphere = new SphereModel();
                sphere->Load(iss);
                //mModel.push_back(sphere);
            }
            else if( result == "path" )
			{
				Path* path = new Path();
				path->Load(iss);
                mPath.push_back(path);
			}
            else if( result == "spline" )
			{
				BSpline* path = new BSpline();
				path->Load(iss);
                mSpline.push_back(path);
			}
			else if ( result.empty() == false && result[0] == '#')
			{
				// this is a comment line
			}
			else
			{
				fprintf(stderr, "Error loading scene file... !");
				getchar();
				exit(-1);
			}
	    }
	}
	input.close();

	// Set PATH vertex buffers
	for (vector<Path*>::iterator it = mPath.begin(); it < mPath.end(); ++it)
	{
		// Draw model
		(*it)->CreateVertexBuffer();
	}

    // Set B-SPLINE vertex buffers
    for (vector<BSpline*>::iterator it = mSpline.begin(); it < mSpline.end(); ++it)
	{
		// Draw model
		(*it)->CreateVertexBuffer();
	}
    
	// ----------------------------------------------------------------------------------------------------------------------------------
	//Ning's sheep spawn
	//Transparent sheep loader
	TransparentSheepSpawn();
	// ----------------------------------------------------------------------------------------------------------------------------------

    LoadCameras();
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Ning's ghost sheep sqawn
void World::TransparentSheepSpawn(){
	// Ning's Sheep
	// Sheep was here
	// Final project
	SheepModel* character = new SheepModel();
	//character->SetPosition(vec3(0.0f, -10.5f, 0.0f));
	mSheep.push_back(character);
	CubeModel * sacrifice = new CubeModel();
	sacrifice->SetPosition(vec3(0.0f, -20.0f, 0.0f));
	mSacrifice.push_back(sacrifice);
	// Final project
}
// ----------------------------------------------------------------------------------------------------------------------------------

void World::LoadCameras()
{
    // Setup Camera
    mCamera.push_back(new StaticCamera(vec3(3.0f, 5.0f, 5.0f),  vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)));
    mCamera.push_back(new StaticCamera(vec3(3.0f, 30.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)));
    mCamera.push_back(new StaticCamera(vec3(0.5f,  0.5f, 5.0f), vec3(0.0f, 0.5f, 0.0f), vec3(0.0f, 1.0f, 0.0f)));
    
    // WormModel Character controlled with Third Person Camera
	Model* character = World::GetInstance()->findMesh("Worm");
    //character->SetPosition(vec3(0.0f, 0.5f, 0.0f));
    mCamera.push_back(new ThirdPersonCamera(character));
    mModel.push_back(character);
    
    // BSpline Camera
    BSpline* spline = FindSpline("\"RollerCoaster\"");
    if (spline == nullptr)
    {
        spline = FindSplineByIndex(0);
    }
    
    if (spline != nullptr)
    {
        mCamera.push_back(new BSplineCamera(spline , 5.0f));
    }
    
    mCurrentCamera = 0;
}

Path* World::FindPath(ci_string pathName)
{
    for(std::vector<Path*>::iterator it = mPath.begin(); it < mPath.end(); ++it)
    {
        if((*it)->GetName() == pathName)
        {
            return *it;
        }
    }
    return nullptr;
}

BSpline* World::FindSpline(ci_string pathName)
{
    for(std::vector<BSpline*>::iterator it = mSpline.begin(); it < mSpline.end(); ++it)
    {
        if((*it)->GetName() == pathName)
        {
            return *it;
        }
    }
    return nullptr;
}

BSpline* World::FindSplineByIndex(unsigned int index)
{
    return mSpline.size() > 0 ? mSpline[index % mSpline.size()] : nullptr;
}

Model* World::FindModelByIndex(unsigned int index)
{
    return mModel.size() > 0 ? mModel[index % mModel.size()] : nullptr;
}

void World::meshExplostion(){
	//Model* cat = findMesh("Cat");
	//cat->SetScaling(1.0002f * (cat->GetScaling()));
}

void World::init(){
	//Random number
	srand(time(NULL));
	//auto testMesh = assetsManager->loadMesh("../Objects/cat/cat.obj");
	auto wormMesh = assetsManager->loadMesh("../PacFinal.obj");

	//auto fenceMesh = assetsManager->loadMesh("../fence.obj");
	//auto sheepMesh = assetsManager->loadMesh(".. / sheep1.obj");


	World* world = World::GetInstance();
	//Model* testModel = world->CreateModel<MeshModel>("Cat", testMesh);
	Model* wormModel = world->CreateModel<MeshModel>("Worm", wormMesh);
	//Model* fenceModel = world->CreateModel<MeshModel>("Fence", fenceMesh);
	//world->CreateModel<MeshModel>("Sheep", sheepMesh);
	//testModel->SetScaling(glm::vec3(2));
	wormModel->SetScaling(glm::vec3(0.5));
	wormModel->SetPosition((wormModel->GetPosition() + vec3(0.0, 1.0, 0.0)));




	//auto sheepMesh = assetsManager->loadMesh("../sheep1.obj");
	//Model* sheepModel = world->CreateModel<MeshModel>("Sheep", sheepMesh);
	//sheepModel->SetScaling(glm::vec3(0.025));
	//world->SheepSpawn(false);

	//Loads the Landscape
	auto LandTestMesh = assetsManager->loadMesh("../VS2012/Objects/Mountain1.obj");

	auto LandTestModel = world->CreateModel<MeshModel>("Mountain",LandTestMesh);

	LandTestModel->SetScaling(glm::vec3(7.5));
	LandTestModel->SetPosition(glm::vec3(10, -1, 0));
	treeSpawn(5);


	
	// Loop to generate first set of fence parallel (opposite) to each other 
	int x = 13; // Start position for first fence section
	int z = 51; // Start position for first fence section
	int x2 = -39; // Start position for second fence section opposite side
	int z2 = -53.5; // Start position for the second fence section opposite side

	for (int i = 0; i < 23; i++) {

		if (i >= 0 && i <= 10) {

			auto FenceMesh = assetsManager->loadMesh("../fence.obj");
			auto FenceModel = world->CreateModel<MeshModel>("Fence", FenceMesh);
			FenceModel->SetPosition(glm::vec3(x, -1.5, z));
			x -= 5;

		}

		if (i > 10) {

			auto FenceMesh1 = assetsManager->loadMesh("../fence.obj");
			auto FenceModel1 = world->CreateModel<MeshModel>("Fence1", FenceMesh1);
			FenceModel1->SetPosition(glm::vec3(x2, -1.5, z2));
			x2 += 5;
		}
		
	}

	// Another loop to generate second set of fence parallel (opposite) to each other
	int x3 = 16;
	int z3 = 51;
	int x4 = -41;
	int z4 = -45;
	for (int i = 0; i < 40; i++) {
		if (i >= 0 && i <= 19) {

			auto FenceMesh2 = assetsManager->loadMesh("../fence.obj");
			auto FenceModel2 = world->CreateModel<MeshModel>("Fence2", FenceMesh2);
			FenceModel2->SetPosition(glm::vec3(0, -1.5, 0));
			FenceModel2->SetRotation(glm::vec3(FenceModel2->GetPosition()), 85.0);
			FenceModel2->SetPosition(glm::vec3(x3, -1.5, z3));
			z3 -= 5;
		}

		if (i > 19) {

			auto FenceMesh3 = assetsManager->loadMesh("../fence.obj");
			auto FenceModel3 = world->CreateModel<MeshModel>("Fence3", FenceMesh3);
			FenceModel3->SetPosition(glm::vec3(0, -1.5, 0));
			FenceModel3->SetRotation(glm::vec3(FenceModel3->GetPosition()), 85.0);
			FenceModel3->SetPosition(glm::vec3(x4, -1.5, z4));
			z4 += 5;

		}
	}

	// Fence complete

	//Loads Tree

	/*for (int i = 1; i < 5; i++){


		int randomNumberX = rand() % (13-(-41)) + (-41);
		int randomNumberZ = rand() % (51-(-51)) + (-51);

		auto TreeMesh = assetsManager->loadMesh("../VS2012/Objects/Tree.obj");		
		auto TreeModel = world->CreateModel<MeshModel>("Tree", TreeMesh);	
		TreeModel->SetScaling(glm::vec3(15));
		TreeModel->SetPosition(glm::vec3(randomNumberX, .25, randomNumberZ));

	}*/

	
	
}

Model* World::findMesh(std::string name){
	std::vector<Meshes*>::iterator it;

	for (it = mMeshes.begin(); it != mMeshes.end(); it++){
		if (name.compare((*it)->strName) == 0){
			return (*it)->mesh;
		}
	}
	return NULL;
}

void World::SheepSpawn(bool particle){
	srand(time(NULL));
	Model* sheepModel = World::GetInstance()->findMesh("Sheep");
	if (sheepModel != NULL){
		float xpos, zpos;
		do{
			if (particle){
				//nings partical code
			}
			else{
				xpos = rand() % (13 - (-41)) + (-41);
				zpos = rand() % (51 - (-51)) + (-51);
				printf("%f %f\n", xpos, zpos);
				sheepModel->SetPosition(vec3(xpos, 0.0, zpos));
			}
		} while ((xpos < 5 && xpos > -5) && (zpos < 5 && zpos > -5));
	}

	//Ning's Model

}
void World::treeSpawn(int num){
	float xpos, zpos;
	for (int i = 0; i < num; i++){
		do{
			//Loads Tree
			xpos = rand() % (13 - (-41)) + (-41);
			zpos = rand() % (51 - (-51)) + (-51);
		} while ((xpos < 5 && xpos > -5) && (zpos < 5 && zpos > -5));
		auto TreeMesh = assetsManager->loadMesh("../VS2012/Objects/Tree.obj");
		auto TreeModel = World::GetInstance()->CreateModel<MeshModel>("Tree", TreeMesh);
		TreeModel->SetScaling(glm::vec3(15));
		TreeModel->SetPosition(glm::vec3(xpos, .25, zpos));
	}
}


