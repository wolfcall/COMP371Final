//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include "Model.h"
#include <GL/glew.h>

class SheepModel : public Model
{
public:
	SheepModel();
	virtual ~SheepModel();

	virtual void Update(float dt);
	virtual void Draw();
	void ResetPosition();

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:
	// The vertex format could be different for different types of models
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
	};

	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;
	
	// Final project //obj files
	std::vector<glm::vec3> vertices; // Array
	std::vector<glm::vec2> uvs;	// Array
	std::vector<glm::vec3> normals; // Array // Won't be used at the moment.
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint Texture;	// Texture file
	GLuint TextureID; // the ID of texture in shader
	GLuint programID;

	// For Rotation
	float mRotationSpeed; 
	float mRotationAngle;

	// For transparency
	float alpha;

	// Final project ends
};
