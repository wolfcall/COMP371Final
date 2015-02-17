//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "Model.h"
#include "Path.h"
#include "World.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtx/vector_angle.hpp>

using namespace std;
using namespace glm;

Model::Model() : mName("UNNAMED"), mPosition(0.0f, 0.0f, 0.0f), mScaling(1.0f, 1.0f, 1.0f), mRotationAxis(0.0f, 1.0f, 0.0f), mRotationAngleInDegrees(0.0f), mPath(nullptr), mSpeed(0.0f), mTargetWaypoint(0)
{
}

Model::~Model()
{
}

void Model::Update(float dt)
{
	if (mPath)
	{
		glm::vec3 dest = mPath->GetWaypoint(mTargetWaypoint);
		glm::vec3 velocity = dest - mPosition;
		velocity = glm::normalize(velocity);
		glm::vec3 add = dt * mSpeed * velocity;
		if (glm::length(dest - mPosition) < glm::length(add))
		{
			mPosition = dest;
			++mTargetWaypoint;
		}
		else
		{
			mPosition += add;
		}
	}
	// @TODO 4 - If the model moves along a Path, update its position
	//           towards the next waypoint according to the speed.
	//           If you go beyond the waypoint, update the target
	//           waypoint to the next one

}

void Model::Draw()
{
}


void Model::Load(ci_istringstream& iss)
{
	ci_string line;

	// Parse model line by line
	while(std::getline(iss, line))
	{
		// Splitting line into tokens
		ci_istringstream strstr(line);
		istream_iterator<ci_string, char, ci_char_traits> it(strstr);
		istream_iterator<ci_string, char, ci_char_traits> end;
		vector<ci_string> token(it, end);

		if (ParseLine(token) == false)
		{
			fprintf(stderr, "Error loading scene file... token:  %s!", token[0].c_str());
			getchar();
			exit(-1);
		}
	}
}

bool Model::ParseLine(const std::vector<ci_string> &token)
{
		if (token.empty() == false)
	{
		if (token[0].empty() == false && token[0][0] == '#')
		{
			return true;
		}
		else if (token[0] == "name")
		{
			assert(token.size() > 2);
			assert(token[1] == "=");

			mName = token[2];	
		}
		else if (token[0] == "position")
		{
			assert(token.size() > 4);
			assert(token[1] == "=");

			mPosition.x = static_cast<float>(atof(token[2].c_str()));
			mPosition.y = static_cast<float>(atof(token[3].c_str()));
			mPosition.z = static_cast<float>(atof(token[4].c_str()));
		}
		else if (token[0] == "rotation")
		{
			assert(token.size() > 4);
			assert(token[1] == "=");

			mRotationAxis.x = static_cast<float>(atof(token[2].c_str()));
			mRotationAxis.y = static_cast<float>(atof(token[3].c_str()));
			mRotationAxis.z = static_cast<float>(atof(token[4].c_str()));
			mRotationAngleInDegrees = static_cast<float>(atof(token[5].c_str()));

			glm::normalize(mRotationAxis);
		}
		else if (token[0] == "scaling")
		{
			assert(token.size() > 4);
			assert(token[1] == "=");

			mScaling.x = static_cast<float>(atof(token[2].c_str()));
			mScaling.y = static_cast<float>(atof(token[3].c_str()));
			mScaling.z = static_cast<float>(atof(token[4].c_str()));
		}
        else if (token[0] == "pathspeed")
		{
			assert(token.size() > 2);
			assert(token[1] == "=");

            float speed = static_cast<float>(atof(token[2].c_str()));
            SetSpeed(speed);
		}
        else if (token[0] == "boundpath")
		{
			assert(token.size() > 2);
			assert(token[1] == "=");

			ci_string pathName = token[2];
            World* w = World::GetInstance();
            mPath = w->FindPath(pathName);
			mTargetWaypoint = 1;
			if (mPath != nullptr)
			{
				mPosition = mPath->GetWaypoint(0);
			}
		}
		else
		{
			return false;
		}
	}

	return true;
}

glm::mat4 Model::GetWorldMatrix() const
{
	mat4 worldMatrix(1.0f);
	worldMatrix = glm::translate(worldMatrix, this->mPosition);
	worldMatrix = glm::rotate(worldMatrix, this->mRotationAngleInDegrees, this->mRotationAxis);
	worldMatrix = glm::scale(worldMatrix, this->mScaling);
	return worldMatrix;
}

void Model::SetPosition(glm::vec3 position)
{
	mPosition = position;
}

void Model::SetScaling(glm::vec3 scaling)
{
	mScaling = scaling;
}

void Model::SetRotation(glm::vec3 axis, float angleDegrees)
{
	mRotationAxis = axis;
	mRotationAngleInDegrees = angleDegrees;
}

void Model::SetSpeed(float spd)
{
    mSpeed = spd;
}