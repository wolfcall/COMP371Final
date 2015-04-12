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
// Structure for each particle
struct Particle{
	glm::vec3 pos, speed;
	unsigned char r, g, b, a; // Color
	float size, angle, weight; // Random size. angle & weight are not used.
	float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

	// Algorithm for sorting: define <
	// cameraDistance is large --> particle is small
	// Soring default: ascendent --> particle from small to large --> cameraDistance from large to small
	bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

// Capacity of particles for each sheep particle model
const int MaxParticles = 25; 


// Inheritance from Model
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
	// 4 vertices of the particle. will be shared by all particles.
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
	};

	// Final project //obj files
	GLuint TextureID;
	GLuint squareVerticesID;
	GLuint xyzsID;
	GLuint colorID;

	// 2 arrays. only alive particles are in these 2 arrays.
	// 1 particle using 4 index // used size is 4*ParticlesCount
	GLfloat* g_particule_position_size_data;	// xyzs	// array --> buffer -->shader
	// 1 particle using 4 index // used size is 4*ParticlesCount
	GLubyte* g_particule_color_data;	// rgba	// array --> buffer -->shader

	GLuint Texture; // Texture --> TextureID (draw)
	GLuint billboard_vertex_buffer;	// 4 vertices // g_vertex_buffer_data[] --> billboard_vertex_buffer (constructor) // buffer --> shader (draw)
	GLuint particles_position_buffer;// g_particule_position_size_data[] --> particles_position_buffer (draw) // buffer --> shader (draw)
	GLuint particles_color_buffer; // g_particule_color_data[] --> particles_color_buffer (draw) // buffer --> shader (draw)


	// Particle struct
	// 1 particle using 1 index
	Particle ParticlesContainer[MaxParticles];	// 1 SheepParticleModel has only 1 ParticlesContainer[], containing alive & dead particles.

	int FindUnusedParticle();	// Find border between alive & dead
	int LastUsedParticle = 0;	// Index of border between alive & dead // border is 1st dead
	void SortParticles(); // Left: alive  Right: dead
	glm::mat4 mViewMatrix;	// used to calculate cameradistance

	int newparticles = 1000;	// In the 1st frame, 1 SheepParticleModel actually draw 1000 particles
	int ParticlesCount;	// Initial value: 1000// Amount of alive particles actually displayed in each frame

	// Final project ends
};
