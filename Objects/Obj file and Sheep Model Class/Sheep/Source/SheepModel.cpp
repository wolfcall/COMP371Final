//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "SheepModel.h"
#include "Renderer.h"
<<<<<<< HEAD
#include "SheepTexture.hpp"
#include "SheepObjloader.hpp"
=======
#include "texture.hpp"
#include "objloader.hpp"
>>>>>>> master


// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>

using namespace glm;

<<<<<<< HEAD
// For shading. Should be in the same file with Uniform
// Material Coefficients
const float ka = 0.5f;
const float kd = 0.8f;
const float ks = 0.2f;
const float n = 50.0f;

// Light Coefficients
const vec3 lightColor(1.0f, 1.0f, 1.0f); // white
const float lightKc = 0.0f; // f att: constant
const float lightKl = 0.0f; // f att: linear
const float lightKq = 1.0f; // f att: quatratic

// const vec4 lightPosition(0.0f, 300.0f, 0.0f, 1.0f); // If w = 1.0f, we have a point light
const vec4 lightPosition(0.0f, -5.0f, 0.0f, 0.0f); // If w = 0.0f, we have a directional light

=======
>>>>>>> master
SheepModel::SheepModel(vec3 size) : Model()
{
	// Set Shader for Sheep
	unsigned int prevShader = Renderer::GetCurrentShader();
	Renderer::SetShader(SHADER_SHEEP);

	// Load the texture
<<<<<<< HEAD
	Texture = loadDDS("../sheep1.dds");
=======
	Texture = loadDDS("../uvmap.DDS");
>>>>>>> master
	// Create and compile our GLSL program from the shaders
	programID = Renderer::GetShaderProgramID();
	// Get a handle for our "myTextureSampler" uniform
	TextureID = glGetUniformLocation(programID, "myTextureSampler");


	// Read our .obj file
	//std::vector<glm::vec3> vertices;
	//std::vector<glm::vec2> uvs;
	//std::vector<glm::vec3> normals; // Won't be used at the moment.
<<<<<<< HEAD
	bool res = loadOBJ("../sheep1.obj", vertices, uvs, normals);
	SetScaling(vec3(0.03f, 0.03f, 0.03f));
=======
	bool res = loadOBJ("../cube.obj", vertices, uvs, normals);

>>>>>>> master

	// Create a vertex array
	glGenVertexArrays(1, &mVertexArrayID);

	// Load it into a VBO
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW); // uv means: relation of object's vertices and texture

<<<<<<< HEAD
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec2), &normals[0], GL_STATIC_DRAW);

	//watch
	//printf("vertex size: %u\n uv size:%u\n", vertices.size(), uvs.size());
	//printf("GetCurrentShader:%u\n", Renderer::GetCurrentShader());
=======
	//watch
	printf("vertex size: %u\n uv size:%u\n", vertices.size(), uvs.size());
	printf("GetCurrentShader:%u\n", Renderer::GetCurrentShader());
>>>>>>> master

	// Restore previous shader
	Renderer::SetShader((ShaderType)prevShader);
}

SheepModel::~SheepModel()
{
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
<<<<<<< HEAD
	glDeleteBuffers(1, &normalbuffer);
=======
>>>>>>> master
	//glDeleteProgram(programID);
	glDeleteTextures(1, &TextureID);
	//glDeleteVertexArrays(1, &VertexArrayID);
}

void SheepModel::Update(float dt)
{
	// If you are curious, un-comment this line to have spinning cubes!
	// That will only work if your world transform is correct...
	// mRotationAngleInDegrees += 90 * dt; // spins by 90 degrees per second
<<<<<<< HEAD
	mRotationAngle += mRotationSpeed * dt;
	if (mRotationAngle > 360)	{
		mRotationAngle -= 360;
	}
	SetRotation(vec3(0.0f, 1.0f, 0.0f), mRotationAngle);
=======
>>>>>>> master

	Model::Update(dt);
}

void SheepModel::Draw()
{
	//watch
	//printf("GetCurrentShader:%u\n", Renderer::GetCurrentShader());
<<<<<<< HEAD
	//printf("Sheep Position: %f,%f,%f\n", mPosition[0], mPosition[1], mPosition[2]);
=======
	printf("Sheep Position: %f,%f,%f\n", mPosition[0], mPosition[1], mPosition[2]);
>>>>>>> master

	// Draw the Vertex Buffer
	// Note this draws a unit Cube
	// The Model View Projection transforms are computed in the Vertex Shader
	programID = Renderer::GetShaderProgramID();

	glBindVertexArray(mVertexArrayID);

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform"); 
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);
	
<<<<<<< HEAD
	GLuint ModelSunLightPositionID = glGetUniformLocation(Renderer::GetShaderProgramID(), "SunLightPosition_modelspace");
	glUniform4f(ModelSunLightPositionID, lightPosition.x, lightPosition.y, lightPosition.z, lightPosition.w);

	// Get a handle for Light Attributes uniform
	GLuint LightColorID = glGetUniformLocation(Renderer::GetShaderProgramID(), "lightColor");
	GLuint LightAttenuationID = glGetUniformLocation(Renderer::GetShaderProgramID(), "lightAttenuation");
	// Get a handle for Material Attributes uniform
	GLuint MaterialID = glGetUniformLocation(Renderer::GetShaderProgramID(), "materialCoefficients");

	// Set shader constants
	glUniform4f(MaterialID, ka, kd, ks, n);
	glUniform3f(LightColorID, lightColor.r, lightColor.g, lightColor.b);
	glUniform3f(LightAttenuationID, lightKc, lightKl, lightKq);

=======
>>>>>>> master
	// Bind our texture in Texture Unit 0
	//glUseProgram(programID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(TextureID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size // vertex is 3-dimension
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size // uv is 2-dimenstion
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

<<<<<<< HEAD
	// 3rd attribute buffer : Normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		2,                                // size // uv is 2-dimenstion
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

=======
>>>>>>> master
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
<<<<<<< HEAD
	glDisableVertexAttribArray(2);
=======
>>>>>>> master
}

bool SheepModel::ParseLine(const std::vector<ci_string> &token)
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
