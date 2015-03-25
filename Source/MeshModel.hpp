#pragma once

#include "Model.h"
#include "ObjMesh.hpp"

class MeshModel : public Model
{
public:
	MeshModel(std::shared_ptr<ObjMesh> mesh)
		: _mesh(mesh)
	{
		assert(_mesh != nullptr && "Give a valid mesh pliz");
	}

	virtual ~MeshModel(void)
	{}

	virtual void Update(float dt)
	{
		Model::Update(dt);
	}

	virtual void Draw()
	{
		assert(_mesh);

		for (auto &e : _mesh->getSubMeshs())
		{
			// Draw the Vertex Buffer
			// Note this draws a unit Cube
			// The Model View Projection transforms are computed in the Vertex Shader
			glBindVertexArray(e._vertexArrayID);

			GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
			glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);

			// 1st attribute buffer : vertex Positions
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, e._vertexBufferID);
			glVertexAttribPointer(0,              // attribute. No particular reason for 0, but must match the layout in the shader.
				3,              // size
				GL_FLOAT,       // type
				GL_FALSE,       // normalized?
				sizeof(glm::vec3) * 4, // stride
				(void*)0        // array buffer offset
				);

			// 2nd attribute buffer : vertex normal
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, e._vertexBufferID);
			glVertexAttribPointer(1,
				3,
				GL_FLOAT,
				GL_FALSE,
				sizeof(glm::vec3) * 4,
				(void*)sizeof(glm::vec3)    // Normal is Offseted by vec3 (see class Vertex)
				);


			// 3rd attribute buffer : vertex color
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, e._vertexBufferID);
			glVertexAttribPointer(2,
				3,
				GL_FLOAT,
				GL_FALSE,
				sizeof(glm::vec3) * 4,
				(void*)(2 * sizeof(glm::vec3)) // Color is Offseted by 2 vec3 (see class Vertex)
				);

			// 4th attribute buffer : UVs
			glEnableVertexAttribArray(3);
			glBindBuffer(GL_ARRAY_BUFFER, e._vertexBufferID);
			glVertexAttribPointer(3,
				3,
				GL_FLOAT,
				GL_FALSE,
				sizeof(glm::vec3) * 4,
				(void*)(3 * sizeof(glm::vec3))
				);

			// Draw the triangles !
			glDrawArrays(GL_TRIANGLES, 0, e._numOfVertices);

			glDisableVertexAttribArray(3);
			glDisableVertexAttribArray(2);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(0);
		}
	}

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token) { (void)(token); return true; }

private:
	std::shared_ptr<ObjMesh> _mesh = nullptr;
};