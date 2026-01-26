#include "Mesh.hpp"
#include "Camera.hpp"

Mesh::Mesh(const std::vector<Vertex>& verts, const std::vector<U32>& indis, ID3D11Device* device)
	: m_vertexBuffer(device, verts.data(), sizeof(Vertex), U32(verts.size() * sizeof(Vertex))),
	m_indexBuffer(device, indis.data(), static_cast<U32>(indis.size())), m_cBuffer(device)
{
	std::wstring path = L"..\\DX11Engine\\shaders\\";

	m_vertexShader = DX11VertexShader(device, path, L"SimpleVShader.hlsl", VertexLayouts::PositionNormalTexcoord::Desc, VertexLayouts::PositionNormalTexcoord::Count);
	m_pixelShader = DX11PixelShader(device, path, L"SimplePShader.hlsl");

}

void Mesh::Draw(ID3D11DeviceContext* context) const
{
	assert(context);

	m_vertexShader.Bind(context);

	m_pixelShader.Bind(context);

	DX::XMMATRIX mat = DX::XMMatrixIdentity();

	CBDMatrix data{};
	data.normal = DX::XMMatrixTranspose(DX::XMMatrixInverse(nullptr, mat));
	data.model = DX::XMMatrixTranspose(mat);
	data.mvp = DX::XMMatrixTranspose(mat * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());

	m_cBuffer.BindVS(context, data);

	m_vertexBuffer.Bind(context);
	m_indexBuffer.Bind(context);

	context->DrawIndexed(m_indexBuffer.GetIndexCount(), 0, 0);
}