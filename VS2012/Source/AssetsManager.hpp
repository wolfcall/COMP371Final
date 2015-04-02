#pragma once

#include <memory>
#include "Model_Classes/ObjMesh.hpp"
#include "tiny_obj_loader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../ThirdParty/soil/SOIL.h"

//quick functions - to remove
float randomColor()
{
	return (float)(rand() % 100) / 100.0f;
}

glm::vec3 randomVec3Color()
{
	return glm::vec3(randomColor(), randomColor(), randomColor());
}

std::string GetFile(const std::string& str)
{
	unsigned found = str.find_last_of("/\\");
	return str.substr(found + 1);
}

std::string GetFolder(const std::string& str)
{
	unsigned found = str.find_last_of("/\\");
	return str.substr(0, found) + "/";
}
//------------

class AssetsManager
{
private:
	AssetsManager()
	{}

	std::map<std::string, std::shared_ptr<ObjMesh> > _meshs;
	std::map<std::string, GLuint> _textures;
public:
	static AssetsManager *getInstance()
	{
		static std::unique_ptr<AssetsManager> ptr = nullptr;
		if (!ptr)
		{
			ptr = std::unique_ptr<AssetsManager>(new AssetsManager());
		}
		return ptr.get();
	}

	GLuint loadTexture(const std::string &path)
	{
		if (_textures.find(path) != _textures.end())
		{
			return _textures[path];
		}

		GLuint tex_2d = SOIL_load_OGL_texture
			(
			path.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
			);

		/* check for an error during the load process */
		if (0 == tex_2d)
		{
			return -1;
		}
		_textures.insert(std::make_pair(path, tex_2d));
		return tex_2d;
	}

	std::shared_ptr < ObjMesh > loadMesh(const std::string &path)
	{
		if (_meshs.find(path) != _meshs.end())
		{
			return _meshs[path];
		}

		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string err = tinyobj::LoadObj(shapes, materials, path.c_str(), GetFolder(path).c_str());

		if (!err.empty())
		{
			std::cerr << err << std::endl;
			return nullptr;
		}


		auto mesh = std::make_shared<ObjMesh>();

		_meshs.insert(std::make_pair(path, mesh));

		mesh->_subMeshs.resize(shapes.size());
		mesh->_subMaterials.resize(materials.size());

		
		for (size_t i = 0; i < materials.size(); i++)
		{
			auto &sub = mesh->_subMaterials[i];

			//printf("material[%ld].name = %s\n", i, materials[i].name.c_str());
			sub.colorAmbiant = glm::vec3(materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]);
			sub.colorDiffuse = glm::vec3(materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
			sub.colorSpecular = glm::vec3(materials[i].specular[0], materials[i].specular[1], materials[i].specular[2]);
			sub.colorTransmittance = glm::vec3(materials[i].transmittance[0], materials[i].transmittance[1], materials[i].transmittance[2]);
			sub.colorEmission = glm::vec3(materials[i].emission[0], materials[i].emission[1], materials[i].emission[2]);

			if (materials[i].ambient_texname.size() > 0)
			{
				sub.textureAmbiant = loadTexture(GetFolder(path) + materials[i].ambient_texname);
			}
			else
			{
				sub.textureAmbiant = loadTexture("../Objects/white.bmp");
			}
			if (materials[i].diffuse_texname.size() > 0)
			{
				sub.textureDiffuse = loadTexture(GetFolder(path) + materials[i].diffuse_texname);
			}
			else
			{
				sub.textureDiffuse = loadTexture("../Objects/white.bmp");
			}
			if (materials[i].specular_texname.size() > 0)
			{
				sub.textureSpecular = loadTexture(GetFolder(path) + materials[i].specular_texname);
			}
			else
			{
				sub.textureSpecular = loadTexture("../Objects/white.bmp");
			}
			if (materials[i].normal_texname.size() > 0)
			{
				sub.textureNormal = loadTexture(GetFolder(path) + materials[i].normal_texname);
			}
			else
			{
				sub.textureNormal = loadTexture("../Objects/white.bmp");
			}
		}
		

		//------------------------

		std::vector<glm::vec3> tmp;

		for (auto i = 0; i < shapes.size(); ++i)
		{
			auto &sub = mesh->_subMeshs[i];
			auto &shape = shapes[i];
			auto &mesh = shape.mesh;

			tmp.clear();
			tmp.reserve(mesh.indices.size() * 3); // 3 == positions + normals + UVs -> very dirty cause Uvs use vec3 ... okay for now :)


			sub.materialId = mesh.material_ids.size() > 0 ? mesh.material_ids[0] : -1;

			for (size_t f = 0; f < mesh.indices.size(); f++)
			{
					auto x = mesh.indices[f] * 3;
					auto y = mesh.indices[f] * 2;

					if (mesh.positions.empty())
					{
						// should not appened normally :)
						tmp.push_back(glm::vec3(0.0f));
					}
					else
					{
						tmp.push_back(glm::vec3(mesh.positions[x + 0], mesh.positions[x + 1], mesh.positions[x + 2]));
					}

					if (mesh.normals.empty())
					{
						tmp.push_back(glm::vec3(1));
					}
					else
					{
						tmp.push_back(glm::vec3(mesh.normals[x + 0], mesh.normals[x + 1], mesh.normals[x + 2]));
					}

					if (mesh.texcoords.empty())
					{
						tmp.push_back(glm::vec3(1, 1, 1));
					}
					else
					{
						tmp.push_back(glm::vec3(mesh.texcoords[y], mesh.texcoords[y + 1], 0));
					}
			}

			sub._numOfVertices = mesh.indices.size();
			glGenVertexArrays(1, &sub._indexArrayID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sub._indexArrayID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);

			glGenVertexArrays(1, &sub._vertexArrayID);
			glGenBuffers(1, &sub._vertexBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, sub._vertexBufferID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * tmp.size(), glm::value_ptr(tmp.front()), GL_STATIC_DRAW);

		}

		return mesh;
	}
};