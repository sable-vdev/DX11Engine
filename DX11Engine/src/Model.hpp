#pragma once
#include "Mesh.hpp"

class Model
{
public:
	void Draw(ID3D11DeviceContext* context) const;
public:
	std::string name;
	std::vector<Mesh> meshes;
};
