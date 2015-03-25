#pragma once

class AssetsManager;

class ObjMesh
{
private:

	struct SubObj
	{
		unsigned int _vertexArrayID = 0;
		unsigned int _vertexBufferID = 0;
		unsigned int _numOfVertices = 0;
	};

	struct SubMaterial
	{

	};

	std::vector < SubObj > _subObjs;

	friend class AssetsManager;
};