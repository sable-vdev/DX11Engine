#pragma once
#include "Mesh.hpp"
#include "Texture.hpp"

class Model
{
public:
	void Draw(ID3D11DeviceContext* context) const
	{
		for (const Mesh& mesh : meshes)
		{
			mesh.Draw(context);
		}
	}
public:
	std::string name;
	std::vector<Mesh> meshes;
	ModelTexture textures;
};