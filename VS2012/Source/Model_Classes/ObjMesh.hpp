#pragma once

class AssetsManager;

class ObjMesh
{
public:

	struct SubMesh
	{
		unsigned int _vertexArrayID = 0;
		unsigned int _indexArrayID = 0;
		unsigned int _vertexBufferID = 0;
		unsigned int _numOfVertices = 0;
	};

	struct SubMaterial
	{
		GLuint textureAmbiant = -1;
		GLuint textureDiffuse = -1;
		GLuint textureSpecular = -1;
		GLuint textureNormal = -1;
		glm::vec3 colorAmbiant = glm::vec3(1);
		glm::vec3 colorDiffuse = glm::vec3(1);
		glm::vec3 colorSpecular = glm::vec3(1);
		glm::vec3 colorTransmittance = glm::vec3(1);
		glm::vec3 colorEmission = glm::vec3(1);
	};

	std::vector<SubMesh> &getSubMeshs() { return _subMeshs; }

private:
	std::vector < SubMesh > _subMeshs;
	std::vector < SubMaterial > _subMaterials;

	friend class AssetsManager;
};