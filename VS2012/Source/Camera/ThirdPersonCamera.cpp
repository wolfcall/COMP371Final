//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 28/1/15
//
// CopymRight (c) 2014-2015 Concordia University. All mRights reserved.
//

#include "ThirdPersonCamera.h"
#include "../EventManager.h"
#include "../World.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <GLFW/glfw3.h>
#include <algorithm>


using namespace glm;


ThirdPersonCamera::ThirdPersonCamera(Model* targetModel)
: Camera(), mTargetModel(targetModel), mHorizontalAngle(0.0f), mVerticalAngle(0.0f), mRadius(10.0f), wormSteering(0.0f), wormSteeringOffset(0.0f), inc(0)

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
	// Calculate Camera Vectors (LookAt, Up, mRight) from Spherical Coordinates
	// Convert from Spherical to Cartesian Coordinates to get the lookAt Vector
	if (mVerticalAngle >= 1.57 * 85 / 90) // = pi/2
		mVerticalAngle = 1.57 * 85 / 90;
	if (mVerticalAngle <= -1.57 * 85 / 90)
		mVerticalAngle = -1.57 * 85 / 90; //Vertical clamping

	if (wormSteering > 3.14f)
		wormSteering -= 6.28f; // = 2pi
	if (wormSteering < -3.14f)
		wormSteering += 6.28f; //Horizontal warping

	mLookAt = glm::vec3  //camera
		(cos(mVerticalAngle) * sin(wormSteeringOffset), sin(mVerticalAngle), cos(mVerticalAngle) * cos(wormSteeringOffset));

	wormLookAt = glm::vec3  //camera
		(sin(wormSteering), 0, cos(wormSteering));

	/*
	mAimRotation = glm::vec3  //second lookAt vector for the cube that is locked vertically (to prevent "flying")
	(sin(mHorizontalAngle), 0, cos(mHorizontalAngle)); //Took this out since it wasn't necessary, but can still be useful
	*/

	thirdPersonOffset = vec3(-15, -15, -15) * mLookAt;

	mPosition = mTargetModel->GetPosition() + thirdPersonOffset;

	mRight = glm::vec3(
		sin(wormSteeringOffset - 3.14f / 2.0f), 0, cos(wormSteeringOffset - 3.14f / 2.0f));

	mUp = glm::cross(mRight, mLookAt);//Cross product of LookAt and Right vectors
}

void ThirdPersonCamera::Update(float dt)
{
	// @TODO
	// 1 - Map Mouse motion to Spherical Angles
	// 2 - Clamp vertical angle to [-85, 85] degrees - DONE ABOVE
	// 3 - Wrap Horizontal angle within [-180, 180] degrees - DONE ABOVE

	//Hide the mouse cursor
	EventManager::DisableMouseCursor();
	EventManager* event;
	GLFWwindow* window = event->GetWindow();
	FoV = 45.0f;
	speed = 5.0f;
	mouseSpeed = 0.400f;

	//Mouse Position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	//Reset Mouse position (if it goes off screen it loses focus)
	glfwSetCursorPos(window, (1024 / 2), (768 / 2));

	// Compute new orientation based on mouse position
	mHorizontalAngle += mouseSpeed * dt * float(1024 / 2 - xpos);
	//mVerticalAngle += mouseSpeed * dt * float(768 / 2 - ypos);
	mVerticalAngle = 1.57*-20 / 90;

	// Controls

	// Press W to move Forward
	//if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W ) == GLFW_PRESS)
	//{
	mTargetModel->SetPosition(mTargetModel->GetPosition() + wormLookAt*vec3(1, 0, 1) * dt * speed);
	//}    
	// Press S to move Backwards
	//if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	//{
	//	mTargetModel->SetPosition(mTargetModel->GetPosition() - wormLookAt*vec3(1,0,1) * dt * speed);
	//}
	// Press A to steer left
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		//mTargetModel->SetPosition(mTargetModel->GetPosition() - mRight * dt * speed);
		wormSteering += mouseSpeed * dt * speed;
		if (inc < +20)
		{
			inc++;
		}
		//wormSteeringOffset = wormSteering + mouseSpeed * dt * speed * inc;
	}
	// Press D to steer Right
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		//mTargetModel->SetPosition(mTargetModel->GetPosition() + mRight * dt * speed);
		wormSteering -= mouseSpeed * dt * speed;
		if (inc > -20)
		{
			inc--;
		}
		//wormSteeringOffset = wormSteering - mouseSpeed * dt * speed * inc;
	}
	else { //when no key is pressed, re-orient camera

		if (inc > 0)
		{
			inc--;
		}
		if (inc < 0)
		{
			inc++;
		}
	}
	wormSteeringOffset = wormSteering + inc/40.000;
	// @TODO
	// Align target model with the horizontal angle
	mTargetModel->SetRotation(vec3(0, 1, 0), wormSteering*57.3); //Not working 100% properly

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);

	CalculateCameraBasis();
}

glm::mat4 ThirdPersonCamera::GetViewMatrix() const
{
	return glm::lookAt(mPosition, mPosition + mLookAt, mUp);
}