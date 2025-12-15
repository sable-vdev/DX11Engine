#include "Model.hpp"

Model::Model(const DX11VertexBuffer& vBuffer, const DX11IndexBuffer& iBuffer, const DX11ConstantBuffer<CBDMatrix>& cBuffer, const DX11VertexShader& vShader, const DX11PixelShader& pShader) :
	m_vBuffer(vBuffer), m_iBuffer(iBuffer), m_cBuffer(cBuffer), m_vShader(vShader), m_pShader(pShader)
{
}

Model::~Model()
{
}

void Model::Draw(ID3D11DeviceContext* context)
{
	m_vBuffer.Bind(context);

	CBDMatrix data{};
	data.wvp = m_modelMatrix * Camera::GetViewMatrix();
	data.wvp = DirectX::XMMatrixTranspose(m_modelMatrix * Camera::GetViewMatrix() *Camera::GetProjectionMatrix());

	m_cBuffer.BindVS(context, data);

	m_vShader.Bind(context);
	m_pShader.Bind(context);

	m_iBuffer.DrawIndexed(context);
}
