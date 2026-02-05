#pragma once
#include <memory>

#include "Buffer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "ConstantBufferInfo.hpp"

class Sprite
{
public:
	Sprite(const std::string& filepath);

	void Draw(ID3D11DeviceContext* context);
private:
	float3 m_position = float3(200.0f, 200.0f, 0.0f);
	Vec4 m_rotation = DX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	float3 m_scale = float3(100.0f, 100.0f, 1.0f);

	std::unique_ptr<DX11VertexShader> m_vShader;
	std::unique_ptr<DX11PixelShader> m_pShader;
	std::unique_ptr<DX11VertexBuffer> m_vBuffer;
	std::unique_ptr<DX11IndexBuffer> m_iBuffer;
	std::unique_ptr<Texture> m_texture;
	std::unique_ptr<DX11ConstantBuffer<CBDSprite>> m_cbdSprite;
};

