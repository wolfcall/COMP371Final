//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 28/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include "Camera.h"
#include "../BSpline/BSpline.h"

class ThirdPersonCamera : public Camera
{
public:
    ThirdPersonCamera(Model* targetModel);
    virtual ~ThirdPersonCamera();

    virtual void Update(float dt);
    virtual glm::mat4 GetViewMatrix() const;

private:
    virtual void CalculateCameraBasis();
    Model* mTargetModel;
    
    // Cartesian Coordinates
    float mHorizontalAngle;
	float wormSteering;
	float wormSteeringOffset;
    float mVerticalAngle;
    float mRadius;
	int inc;
    
    // Camera Vectors
    glm::vec3 mPosition;
	//glm::vec3 mAimRotation;
    glm::vec3 mLookAt;
	glm::vec3 wormLookAt;
    glm::vec3 mRight;
    glm::vec3 mUp;
	//glm::vec3 mDirection;
	

    // @TODO - Maybe you should add more camera attributes (Speed, etc)
	float speed;
	float mouseSpeed;
	float FoV;
	glm::vec3 thirdPersonOffset;


};
