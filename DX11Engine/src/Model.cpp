#include "Model.hpp"
#include "Input.hpp"

Model::Model(const DX11VertexBuffer& vBuffer, const DX11IndexBuffer& iBuffer, const DX11ConstantBuffer<CBDMatrix>& cBuffer, const DX11VertexShader& vShader, const DX11PixelShader& pShader, const Texture& tex) :
	m_vBuffer(vBuffer), m_iBuffer(iBuffer), m_cBuffer(cBuffer), m_vShader(vShader), m_pShader(pShader), m_tex(tex)
{
}

Model::~Model()
{
}

void Model::Update(float dt)
{
	m_rotation.y += 1.0f * dt;

	m_modelMatrix = DirectX::XMMatrixIdentity();

	m_modelMatrix *= DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	m_modelMatrix *= DirectX::XMMatrixRotationX(m_rotation.x);
	m_modelMatrix *= DirectX::XMMatrixRotationY(m_rotation.y);
	m_modelMatrix *= DirectX::XMMatrixRotationZ(m_rotation.z);
	m_modelMatrix *= DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

}

void Model::Render(ID3D11DeviceContext* context)
{
	m_vBuffer.Bind(context);

	CBDMatrix data{};
	data.wvp = m_modelMatrix * Camera::GetViewMatrix();
	data.wvp = DirectX::XMMatrixTranspose(m_modelMatrix * Camera::GetViewMatrix() *Camera::GetProjectionMatrix());

	m_cBuffer.BindVS(context, data);

	m_vShader.Bind(context);
	m_pShader.Bind(context);

	m_tex.Bind(context);

	m_iBuffer.DrawIndexed(context);
}
