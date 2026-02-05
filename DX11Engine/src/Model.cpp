#include "Model.hpp"
#include "Application.hpp"
#include "TextureManager.hpp"

Model::Model(ID3D11Device* device) : m_cBuffer(Application::Get().GetDevice()), m_modelMatrix(DX::XMMatrixIdentity())
{
	std::wstring path = L"..\\DX11Engine\\shaders\\";
	vertexShader = std::make_unique<DX11VertexShader>(device, path, L"SimpleVShader.hlsl", VertexLayouts::PositionNormalTexcoord::Desc, VertexLayouts::PositionNormalTexcoord::Count);
	pixelShader = std::make_unique<DX11PixelShader>(device, path, L"SimplePShader.hlsl");

	textures = std::make_unique<ModelTexture>();
}

void Model::Update(float dt)
{
	m_modelMatrix = DX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z)* DX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z)
		* DX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
}

void Model::Draw(ID3D11DeviceContext* context) const
{
	assert(context);

	CBDMatrix data{};
	data.normal = DX::XMMatrixTranspose(DX::XMMatrixInverse(nullptr, m_modelMatrix));
	data.model = DX::XMMatrixTranspose(m_modelMatrix);
	data.mvp = DX::XMMatrixTranspose(m_modelMatrix * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());

	auto tex = TextureManager::Get(textures->diffuse);

	tex->Bind(context);
	m_cBuffer.BindVS(context, data);
	vertexShader->Bind(context);
	pixelShader->Bind(context);

	for (const Mesh& mesh : meshes)
	{
		mesh.Draw(context);
	}
}
