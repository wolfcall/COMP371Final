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

	};

	std::vector<SubMesh> &getSubMeshs() { return _subMeshs; }

private:
	std::vector < SubMesh > _subMeshs;

	friend class AssetsManager;
};