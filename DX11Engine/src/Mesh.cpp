#include "Mesh.hpp"
#include "Camera.hpp"

Mesh::Mesh(const std::vector<Vertex>& verts, const std::vector<U32>& indis, ID3D11Device* device)
	: m_vertexBuffer(device, verts.data(), sizeof(Vertex), U32(verts.size() * sizeof(Vertex))),
	m_indexBuffer(device, indis.data(), static_cast<U32>(indis.size()))
{

}

void Mesh::Draw(ID3D11DeviceContext* context) const
{
	assert(context);

	m_vertexBuffer.Bind(context);
	m_indexBuffer.DrawIndexed(context);
}