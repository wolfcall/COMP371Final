#pragma once

class AssetsManager;

class ObjMesh
{
private:

	struct SubMesh
	{
		unsigned int _vertexArrayID = 0;
		unsigned int _vertexBufferID = 0;
		unsigned int _numOfVertices = 0;
	};

	struct SubMaterial
	{

	};

	std::vector < SubMesh > _subMeshs;

	friend class AssetsManager;
};