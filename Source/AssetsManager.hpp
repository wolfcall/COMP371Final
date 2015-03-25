#pragma once

#include <memory>
#include "ObjMesh.hpp"
#include "tiny_obj_loader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class AssetsManager
{
private:
	AssetsManager()
	{}

	std::map<std::string, std::shared_ptr<ObjMesh> > _meshs;
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

	std::shared_ptr < ObjMesh > loadMesh(const std::string &path)
	{
		if (_meshs.find(path) != _meshs.end())
		{
			return _meshs[path];
		}

		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string err = tinyobj::LoadObj(shapes, materials, path.c_str());

		if (!err.empty())
		{
			std::cerr << err << std::endl;
			return nullptr;
		}

		auto mesh = std::make_shared<ObjMesh>();

		_meshs.insert(std::make_pair(path, mesh));

		mesh->_subMeshs.resize(shapes.size());

		std::vector<glm::vec3> tmp;

		for (auto i = 0; i < shapes.size(); ++i)
		{
			auto &sub = mesh->_subMeshs[i];
			auto &shape = shapes[i];
			auto &mesh = shape.mesh;

			tmp.clear();
			tmp.reserve(mesh.indices.size() * 4); // 4 == positions + normals + colors + UVs -> very dirty cause Uvs use vec3 ... okay for now :)

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

					if (true /*mesh.colors.empty()*/) //@cesar : todo based on materials
					{
						tmp.push_back(glm::vec3(1, 1, 1));
					}
					else
					{
						//tmp.push_back(glm::vec3(mesh.colors[x], mesh.colors[y], mesh.colors[z]));
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

			sub._numOfVertices = mesh.indices.size() / (sizeof(glm::vec3) * 4);

			glGenVertexArrays(1, &sub._vertexArrayID);
			glGenBuffers(1, &sub._vertexBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, sub._vertexBufferID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * tmp.size(), glm::value_ptr(tmp.front()), GL_STATIC_DRAW);

		}

		return mesh;

		// debug to erase
		{
			std::cout << "# of shapes    : " << shapes.size() << std::endl;
			std::cout << "# of materials : " << materials.size() << std::endl;

			for (size_t i = 0; i < materials.size(); i++) {
				printf("material[%ld].name = %s\n", i, materials[i].name.c_str());
				printf("  material.Ka = (%f, %f ,%f)\n", materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]);
				printf("  material.Kd = (%f, %f ,%f)\n", materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
				printf("  material.Ks = (%f, %f ,%f)\n", materials[i].specular[0], materials[i].specular[1], materials[i].specular[2]);
				printf("  material.Tr = (%f, %f ,%f)\n", materials[i].transmittance[0], materials[i].transmittance[1], materials[i].transmittance[2]);
				printf("  material.Ke = (%f, %f ,%f)\n", materials[i].emission[0], materials[i].emission[1], materials[i].emission[2]);
				printf("  material.Ns = %f\n", materials[i].shininess);
				printf("  material.Ni = %f\n", materials[i].ior);
				printf("  material.dissolve = %f\n", materials[i].dissolve);
				printf("  material.illum = %d\n", materials[i].illum);
				printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
				printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
				printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
				printf("  material.map_Ns = %s\n", materials[i].normal_texname.c_str());
				std::map<std::string, std::string>::const_iterator it(materials[i].unknown_parameter.begin());
				std::map<std::string, std::string>::const_iterator itEnd(materials[i].unknown_parameter.end());
				for (; it != itEnd; it++) {
					printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
				}
				printf("\n");
			}
		}
		return nullptr;
	}
};