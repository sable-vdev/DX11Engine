#include "Model.hpp"
#include "Input.hpp"

Model::Model(const DX11VertexBuffer& vBuffer, const DX11IndexBuffer& iBuffer, const DX11ConstantBuffer<CBDMatrix>& cBuffer, const DX11ConstantBuffer<CBDLight>& cLightBuffer,
	const DX11ConstantBuffer<CBDCamera>& cCameraBuffer, const DX11VertexShader& vShader, const DX11PixelShader& pShader, const Texture& tex) :
	m_vBuffer(vBuffer), m_iBuffer(iBuffer), m_cBuffer(cBuffer), m_cLightBuffer(cLightBuffer), m_vShader(vShader), m_pShader(pShader), m_tex(tex), m_cCameraBuffer(cCameraBuffer)
{
}

Model::~Model()
{
}

void Model::Update(float dt)
{

	m_rotation.y += .5f * dt;
	m_modelMatrix = DirectX::XMMatrixIdentity();
	
	m_modelMatrix *= DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	m_modelMatrix *= DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
	m_modelMatrix *= DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
}

void Model::Render(ID3D11DeviceContext* context)
{
	m_vBuffer.Bind(context);

	CBDMatrix data{};
	data.normal = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, m_modelMatrix));
	data.model = DirectX::XMMatrixTranspose(m_modelMatrix);
	data.mvp = DirectX::XMMatrixTranspose(m_modelMatrix * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());

	m_cBuffer.BindVS(context, data);
	CBDLight lightData{};
	lightData.diffuseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	lightData.ambientColor = float4(0.2f, 0.2f, 0.2f, 1.0f);
	lightData.specularColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	lightData.specularPower = 20.0f;
	lightData.lightDirection = DirectX::XMVector4Normalize(Vec4{ 0.0f, 0.0f, 2.0f, 1.0f });

	m_cLightBuffer.BindPS(context, lightData);

	CBDCamera cameraData{};
	cameraData.cameraPosition = Camera::GetConstantBufferInfo().cameraPosition;
	m_cCameraBuffer.BindVS(context, cameraData, 1);

	m_vShader.Bind(context);
	m_pShader.Bind(context);

	m_tex.Bind(context);

	m_iBuffer.DrawIndexed(context);
}
