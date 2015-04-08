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

// CPU representation of a particle
struct Particle{
	glm::vec3 pos, speed;
	unsigned char r, g, b, a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

	bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

const int MaxParticles = 10000;


class SheepParticleModel : public Model
{
public:
	SheepParticleModel();
	virtual ~SheepParticleModel();

	virtual void Update(float dt);
	virtual void Draw();
	void SetViewMatrix(glm::mat4 ViewMatrix);
	bool IsAlive();

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
	GLuint TextureID;
	GLuint squareVerticesID;
	GLuint xyzsID;
	GLuint colorID;

	GLfloat* g_particule_position_size_data;
	GLubyte* g_particule_color_data;

	GLuint Texture;
	GLuint billboard_vertex_buffer;
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;

	// Particle struct
	int FindUnusedParticle();
	glm::mat4 mViewMatrix;
	void SortParticles();
	Particle ParticlesContainer[MaxParticles];
	int LastUsedParticle = 0;
	int newparticles = 1000;
	int ParticlesCount;

	// For Rotation
	float mRotationSpeed; 
	float mRotationAngle = 0;

	// For transparency
	float alpha;

	// Final project ends
};
