// The following code was referenced from http://www.opengl-tutorial.org/
/**
#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <vector>
#include <glm/glm.hpp>
using namespace glm;

bool LoadOBJ(
	const char * path,
	std::vector<glm::vec3> & outVertices,
	std::vector<glm::vec2> & outUvs,
	std::vector<glm::vec3> & outNormals
	);


/**
Not sure if we need this...
bool LoadAssImp(
	const char * path,
	std::vector<unsigned short> & indices,
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals
	);
*/
//#endif
