#include "Sprite.hpp"
#include "Application.hpp"

Sprite::Sprite(const std::string& filepath)
{
	float data[] = 
	{
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
	};

	U32 indices[] =
	{
		0, 1, 2,
		2, 1, 3
	};

	m_position = float3(200.0f, 200.0f, 0.0f);
	m_vBuffer = std::make_unique<DX11VertexBuffer>(Application::Get().GetDevice(), data, sizeof(float) * 5, sizeof(data));
	m_iBuffer = std::make_unique<DX11IndexBuffer>(Application::Get().GetDevice(), indices, sizeof(indices) / sizeof(indices[0]));

	std::wstring path = L"..\\DX11Engine\\shaders\\";
	m_vShader = std::make_unique<DX11VertexShader>(Application::Get().GetDevice(), path, L"SimpleSpriteVShader.hlsl", VertexLayouts::PositionTexcoord::Desc, VertexLayouts::PositionTexcoord::Count);
	m_pShader = std::make_unique<DX11PixelShader>(Application::Get().GetDevice(), path, L"SimpleSpritePShader.hlsl");
	
	m_texture = std::make_unique<Texture>(Application::Get().GetDevice(), filepath);

	m_cbdSprite = std::make_unique<DX11ConstantBuffer<CBDSprite>>(Application::Get().GetDevice());
}

void Sprite::Draw(ID3D11DeviceContext* context)
{
	assert(context);

	Mat4x4 worldMat = DX::XMMatrixIdentity() * DX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) * DX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f) *
		DX::XMMatrixTranslation(m_position.x, m_position.y, 0.0f);

	CBDSprite data{};
	data.mvp = DX::XMMatrixTranspose(worldMat * Camera::GetOrthographicMatrix());
	
	m_cbdSprite->BindVS(context, data);

	m_texture->Bind(context);

	m_vShader->Bind(context);
	m_pShader->Bind(context);

	m_vBuffer->Bind(context);

	m_iBuffer->DrawIndexed(context);
}
