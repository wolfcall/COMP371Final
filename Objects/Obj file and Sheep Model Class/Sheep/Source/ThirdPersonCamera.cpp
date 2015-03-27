//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 28/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "ThirdPersonCamera.h"
#include "EventManager.h"
#include "World.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <GLFW/glfw3.h>
#include <algorithm>


using namespace glm;


ThirdPersonCamera::ThirdPersonCamera(Model* targetModel)
    : Camera(), mTargetModel(targetModel), mHorizontalAngle(0.0f), mVerticalAngle(0.0f), mRadius(10.0f)

{
    assert(mTargetModel != nullptr);
    CalculateCameraBasis();
}

ThirdPersonCamera::~ThirdPersonCamera()
{
}

void ThirdPersonCamera::CalculateCameraBasis()
{
	// @TODO
    // Calculate Camera Vectors (LookAt, Up, Right) from Spherical Coordinates
    // Convert from Spherical to Cartesian Coordinates to get the lookAt Vector
	mPosition = mTargetModel->GetPosition() + vec3(-10.0f, 0.0f, 0.0f); // This is wrong
	mLookAt = vec3(1.0f, 0.0f, 0.0f); // This is wrong
	mRight = vec3(0.0f, 1.0f, 0.0f);// This is wrong
	mUp = vec3(0.0f, 0.0f, 1.0f);// This is wrong

	glm::vec3 mCameraPosition= vec3(cos(mVerticalAngle)*cos(mHorizontalAngle),sin(mVerticalAngle),-cos(mVerticalAngle)*sin(mHorizontalAngle))*mRadius;
	mPosition=mTargetModel->GetPosition()+mCameraPosition;
	mLookAt=mCameraPosition*(-1.0f)/mRadius;
	mRight=glm::cross(mLookAt,glm::vec3(0.0f,1.0f,0.0f));
	mUp=glm::cross(mRight,mLookAt);


}

void ThirdPersonCamera::Update(float dt)
{
    EventManager::DisableMouseCursor();
    
    // @TODO
    // 1 - Map Mouse motion to Spherical Angles
    // 2 - Clamp vertical angle to [-85, 85] degrees
    // 3 - Wrap Horizontal angle within [-180, 180] degrees

	mMovingSpeed=50;
	mRotatingSpeed=0.001;
	float pi=3.14;

	mMouseX=EventManager::GetMouseMotionX();
	mMouseY=EventManager::GetMouseMotionY();
	mVerticalAngle+=mMouseY*mRotatingSpeed;
	mHorizontalAngle-=mMouseX*mRotatingSpeed;
	if(mVerticalAngle>(85*pi/180)){
		mVerticalAngle=(85*pi/180);
	}
	if(mVerticalAngle<(-85*pi/180)){
		mVerticalAngle=(-85*pi/180);
	}
	if(mHorizontalAngle>(180*pi/180)){
		mHorizontalAngle=(180*pi/180);
	}
	if(mHorizontalAngle<(-180*pi/180)){
		mHorizontalAngle=(-180*pi/180);
	}
	


	glm::vec3 mBackward= glm::vec3(cos(mVerticalAngle)*cos(mHorizontalAngle),0,-cos(mVerticalAngle)*sin(mHorizontalAngle));
	mBackward=mBackward/(mRadius*cos(mVerticalAngle));
	glm::vec3 mRight=glm::cross(glm::vec3(0.0f,1.0f,0.0f),mBackward);
	glm::vec3 mForward=glm::cross(glm::vec3(0.0f,1.0f,0.0f),mRight);
	glm::vec3 mLeft=glm::cross(glm::vec3(0.0f,1.0f,0.0f),mForward);


    // Press W to move Forward
    if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W ) == GLFW_PRESS)
    {
		// @TODO - Move Model Forward
		mTargetModel->SetPosition(mTargetModel->GetPosition()+mMovingSpeed*mForward*dt);
    }
    
    // @TODO
    // Press S to move Backwards
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S ) == GLFW_PRESS)
    {
		mTargetModel->SetPosition(mTargetModel->GetPosition()+mMovingSpeed*mBackward*dt);
    }
    // Press A to move left
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A ) == GLFW_PRESS)
    {
		mTargetModel->SetPosition(mTargetModel->GetPosition()+mMovingSpeed*mLeft*dt);
    }
    // Press D to move right
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D ) == GLFW_PRESS)
    {
		mTargetModel->SetPosition(mTargetModel->GetPosition()+mMovingSpeed*mRight*dt);
    }
    // ...

	
	// @TODO
    // Align target model with the horizontal angle

	mTargetModel->SetRotation(glm::vec3(0.0f,1.0f,0.0f),(mHorizontalAngle)*180/pi+180);

    
    CalculateCameraBasis();
}

glm::mat4 ThirdPersonCamera::GetViewMatrix() const
{
    return glm::lookAt(mPosition, mPosition + mLookAt, mUp);
}
