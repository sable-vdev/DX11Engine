#pragma once
#include "Mesh.hpp"
#include "Texture.hpp"

class Model2
{
public:
	Model2() = default;
	
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