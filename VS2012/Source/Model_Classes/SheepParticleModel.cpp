//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "SheepParticleModel.h"
#include "../Renderer.h"
#include "SheepTexture.hpp"
#include "SheepObjloader.hpp"

#define pi 3.14


// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>

using namespace glm;


// The VBO containing the 4 vertices of the particles.
// Thanks to instancing, they will be shared by all particles.
static const GLfloat g_vertex_buffer_data[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,
};

SheepParticleModel::SheepParticleModel() : Model()
{
	// Set Shader for Sheep
	unsigned int prevShader = Renderer::GetCurrentShader();
	Renderer::SetShader(SHADER_SHEEP_PARTICLE);

	// Create and compile our GLSL program from the shaders
	GLuint programID = Renderer::GetShaderProgramID();

	// Vertex shader
	/*
	GLuint CameraRight_worldspace_ID = glGetUniformLocation(programID, "CameraRight_worldspace");
	GLuint CameraUp_worldspace_ID = glGetUniformLocation(programID, "CameraUp_worldspace");
	GLuint ViewProjMatrixID = glGetUniformLocation(programID, "VP");
	*/

	// fragment shader
	TextureID = glGetUniformLocation(programID, "myTextureSampler");

	// Get a handle for our buffers
	squareVerticesID = glGetAttribLocation(programID, "squareVertices");
	xyzsID = glGetAttribLocation(programID, "xyzs");
	colorID = glGetAttribLocation(programID, "color");

	g_particule_position_size_data = new GLfloat[MaxParticles * 4];
	g_particule_color_data = new GLubyte[MaxParticles * 4];

	// Initialize each particle, all are dead.
	for (int i = 0; i<MaxParticles; i++){
		ParticlesContainer[i].life = -1.0f;
		ParticlesContainer[i].cameradistance = -1.0f;
	}


	Texture = loadDDS("../particle.dds");

	// The VBO containing the 4 vertices of the particles.
	// Thanks to instancing, they will be shared by all particles.
	/*static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
	};*/

	// Vertex position in particle spaces (the shape of the particle)(only 4 vertices)
	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);


	// Generate new particles
	// Activate the first 1000 particles, search from left to right, find the 1st dead one and then activate it
	for (int i = 0; i<newparticles; i++){
		int particleIndex = FindUnusedParticle();
		ParticlesContainer[particleIndex].life = ((float) rand()) / RAND_MAX + 2.0f; // This particle will live 2 -3 seconds.
		ParticlesContainer[particleIndex].pos = glm::vec3(0.0f, 0.0f, 0.0f); // Origin of SheepParticleModel space // Then put this origion to sheep

		float spread = 2.0f;
		glm::vec3 maindir = glm::vec3(0.0f, 0.0f, 0.0f); // Initial velocity is 0
		// Very bad way to generate a random direction; 
		// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
		// combined with some user-controlled parameters (main direction, spread, etc)
		/* cube
		glm::vec3 randomdir = glm::vec3(
			(rand() % 2000 - 1000.0f) / 1000.0f, // x: -1 to 1
			(rand() % 2000 - 1000.0f) / 1000.0f, // y: -1 to 1
			(rand() % 2000 - 1000.0f) / 1000.0f	// z: -1 to 1
			);
			*/
		float alpha = (((float)rand()) / RAND_MAX - 0.5) * pi; // (-pi/2, +pi/2)
		float beta = (((float)rand()) / RAND_MAX) * 2 * pi;	// (0, 2pi)
		float r = ((float)rand()) / RAND_MAX; // (0, 1)
		glm::vec3 randomdir = glm::vec3(
			r * cos(alpha) * cos(beta),	// x
			r * sin(alpha),	// y
			-1 * r * cos(alpha) * sin(beta)	// z
			);	// sphere

		ParticlesContainer[particleIndex].speed = maindir + randomdir*spread; // sphere with r (0, 2)


		// Very bad way to generate a random color
		// Particle color
		ParticlesContainer[particleIndex].r = 255;	// white
		ParticlesContainer[particleIndex].g = 255;	// white
		ParticlesContainer[particleIndex].b = 255;	// white
		ParticlesContainer[particleIndex].a = (rand() % 256) / 3; // 255 is opacity

		// Random size of each particle (square)
		ParticlesContainer[particleIndex].size = (rand() % 1000) / 5000.0f + 0.1f; // 0.1*0.1 - 0.3*0.3

	} // End of for loop

	// Restore previous shader
	Renderer::SetShader((ShaderType)prevShader);
}

SheepParticleModel::~SheepParticleModel()
{
	// Free the GPU from the Vertex Buffer
	delete[] g_particule_position_size_data;

	// Cleanup VBO and shader
	glDeleteBuffers(1, &particles_color_buffer);
	glDeleteBuffers(1, &particles_position_buffer);
	glDeleteBuffers(1, &billboard_vertex_buffer);
	glDeleteTextures(1, &Texture);
}


// Private method
// search in ParticlesContainer[]
int SheepParticleModel::FindUnusedParticle(){
	for (int i = LastUsedParticle; i<MaxParticles; i++){
		if (ParticlesContainer[i].life < 0){
			LastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i<LastUsedParticle; i++){
		if (ParticlesContainer[i].life < 0){
			LastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one

}

// Private method
void SheepParticleModel::SortParticles(){
	std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}

// Public method to test if the particle model is dead
bool SheepParticleModel::IsAlive(){
	return ParticlesContainer[0].life>0; // If all particles are dead, then particle model is dead.
}

// Send view matrix of camera to this particle, for sorting
void SheepParticleModel::SetViewMatrix(glm::mat4 ViewMatrix){
	mViewMatrix = ViewMatrix;
	return;
}

void SheepParticleModel::Update(float dt)
{
	// If you are curious, un-comment this line to have spinning cubes!
	// That will only work if your world transform is correct...
	// mRotationAngleInDegrees += 90 * dt; // spins by 90 degrees per second


	// Update all particle including the new create particles
	// Simulate all particles
	ParticlesCount = 0;
	for (int i = 0; i<MaxParticles; i++){

		Particle& p = ParticlesContainer[i]; // shortcut

		if (p.life > 0.0f){

			p.life -= dt;	// Decrease life

			if (p.life > 0.0f){

				// Simulate simple physics : gravity only, no collisions
				p.speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)dt * 0.1f;  // v(t) = v0 + gt	// Average speed: v bar = v0 + gt/2
				p.pos += p.speed * (float)dt;
				p.cameradistance = glm::length(( mViewMatrix*GetWorldMatrix()*vec4(p.pos,1)) - vec4(0.0f, 0.0f, 0.0f, 0.0f));

				// Fill the attibute arrays
				g_particule_position_size_data[4 * ParticlesCount + 0] = p.pos.x;
				g_particule_position_size_data[4 * ParticlesCount + 1] = p.pos.y;
				g_particule_position_size_data[4 * ParticlesCount + 2] = p.pos.z;

				g_particule_position_size_data[4 * ParticlesCount + 3] = p.size;

				g_particule_color_data[4 * ParticlesCount + 0] = p.r;
				g_particule_color_data[4 * ParticlesCount + 1] = p.g;
				g_particule_color_data[4 * ParticlesCount + 2] = p.b;
				g_particule_color_data[4 * ParticlesCount + 3] = p.a;

			} else {
				// Particles that just died will be put at the end of the buffer in SortParticles();
				// -particles step1
				p.cameradistance = -1.0f;
			}

			ParticlesCount++;

		} // End of if
	} // End of for loop

	//watch
	//printf("ParticlesCount:%d\n", ParticlesCount);

	// -particles step2: put behind by sorting
	SortParticles();
	// Update end

	Model::Update(dt);
}

void SheepParticleModel::Draw()
{
	// Draw start
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture); // Texture to UV
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(TextureID, 0);	// Texture --> TextureID

	// Transparent
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use our shader
	GLuint programID = Renderer::GetShaderProgramID();
	glUseProgram(programID);

	GLuint WorldMatrixLocation = glGetUniformLocation(programID, "WorldTransform");
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]); // Each model has its own WorldMatrix

	

	// Same as the billboards tutorial
	/*
	glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	glUniform3f(CameraUp_worldspace_ID, ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);

	glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);
	*/


	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(squareVerticesID);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glVertexAttribPointer(
		squareVerticesID,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(xyzsID);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glVertexAttribPointer(
		xyzsID,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : x + y + z + size => 4
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// 3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(colorID);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glVertexAttribPointer(
		colorID,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : r + g + b + a => 4
		GL_UNSIGNED_BYTE,                 // type
		GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// These functions are specific to glDrawArrays*Instanced*.
	// The first parameter is the attribute buffer we're talking about.
	// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
	glVertexAttribDivisorARB(squareVerticesID, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisorARB(xyzsID, 1); // positions : one per quad (its center)                 -> 1
	glVertexAttribDivisorARB(colorID, 1); // color : one per quad                                  -> 1

	// Draw the particules !
	// This draws many times a small triangle_strip (which looks like a quad).
	// This is equivalent to :
	// for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4), 
	// but faster.
	glDrawArraysInstancedARB(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);	// Each square has 4 vertices

	glDisableVertexAttribArray(squareVerticesID);
	glDisableVertexAttribArray(xyzsID);
	glDisableVertexAttribArray(colorID);
}

bool SheepParticleModel::ParseLine(const std::vector<ci_string> &token)
{
	if (token.empty())
	{
		return true;
	}
	else
	{
		return Model::ParseLine(token);
	}
}
