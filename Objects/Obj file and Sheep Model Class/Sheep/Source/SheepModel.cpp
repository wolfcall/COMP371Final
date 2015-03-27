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
#include "texture.hpp"
#include "objloader.hpp"


// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>

using namespace glm;

SheepModel::SheepModel(vec3 size) : Model()
{
	// Set Shader for Sheep
	unsigned int prevShader = Renderer::GetCurrentShader();
	Renderer::SetShader(SHADER_SHEEP);

	// Load the texture
	Texture = loadDDS("../uvmap.DDS");
	// Create and compile our GLSL program from the shaders
	programID = Renderer::GetShaderProgramID();
	// Get a handle for our "myTextureSampler" uniform
	TextureID = glGetUniformLocation(programID, "myTextureSampler");


	// Read our .obj file
	//std::vector<glm::vec3> vertices;
	//std::vector<glm::vec2> uvs;
	//std::vector<glm::vec3> normals; // Won't be used at the moment.
	bool res = loadOBJ("../cube.obj", vertices, uvs, normals);


	// Create a vertex array
	glGenVertexArrays(1, &mVertexArrayID);

	// Load it into a VBO
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW); // uv means: relation of object's vertices and texture

	//watch
	printf("vertex size: %u\n uv size:%u\n", vertices.size(), uvs.size());
	printf("GetCurrentShader:%u\n", Renderer::GetCurrentShader());

	// Restore previous shader
	Renderer::SetShader((ShaderType)prevShader);
}

SheepModel::~SheepModel()
{
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	//glDeleteProgram(programID);
	glDeleteTextures(1, &TextureID);
	//glDeleteVertexArrays(1, &VertexArrayID);
}

void SheepModel::Update(float dt)
{
	// If you are curious, un-comment this line to have spinning cubes!
	// That will only work if your world transform is correct...
	// mRotationAngleInDegrees += 90 * dt; // spins by 90 degrees per second

	Model::Update(dt);
}

void SheepModel::Draw()
{
	//watch
	//printf("GetCurrentShader:%u\n", Renderer::GetCurrentShader());
	printf("Sheep Position: %f,%f,%f\n", mPosition[0], mPosition[1], mPosition[2]);

	// Draw the Vertex Buffer
	// Note this draws a unit Cube
	// The Model View Projection transforms are computed in the Vertex Shader
	programID = Renderer::GetShaderProgramID();

	glBindVertexArray(mVertexArrayID);

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform"); 
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);
	
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

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
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
