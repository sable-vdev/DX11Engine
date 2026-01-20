#include "Model.hpp"

void Model::Draw(ID3D11DeviceContext* context) const
{
	for (const Mesh& mesh : meshes)
	{
		mesh.Draw(context);
	}
}
