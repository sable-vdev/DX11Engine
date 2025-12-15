#pragma once

#include "Buffer.hpp"
#include "ConstantBufferInfo.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Texture.hpp"

class Model
{
public:
	Model(const DX11VertexBuffer& vBuffer, const DX11IndexBuffer& iBuffer, const DX11ConstantBuffer<CBDMatrix>& cBuffer,
		const DX11VertexShader& vShader, const DX11PixelShader& pShader);

	~Model();

	void Draw(ID3D11DeviceContext* context);
private:
	DX11VertexBuffer m_vBuffer;
	DX11IndexBuffer m_iBuffer;
	DX11ConstantBuffer<CBDMatrix> m_cBuffer;
	DX11VertexShader m_vShader;
	bool m_padding[8];
	DX11PixelShader m_pShader;

	Mat4x4 m_modelMatrix = DirectX::XMMatrixIdentity();
	float4 m_scale = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 m_rotation = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 m_position = float4(0.0f, 0.0f, 0.0f, 1.0f);
};

