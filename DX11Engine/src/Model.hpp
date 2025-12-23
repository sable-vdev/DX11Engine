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
		const DX11ConstantBuffer<CBDLight>& cLightBuffer, const DX11ConstantBuffer<CBDCamera>& cCameraBuffer,
		const DX11VertexShader& vShader, const DX11PixelShader& pShader, const Texture& tex);

	~Model();

	void Update(float dt);
	void Render(ID3D11DeviceContext* context);

	float3 Scale() const { return float3(m_scale.x, m_scale.y, m_scale.z); }
	float3 Rotation() const { return float3(m_rotation.x, m_rotation.y, m_rotation.z); }
	float4& Position() { return m_position; }

private:
	DX11VertexBuffer m_vBuffer;
	DX11IndexBuffer m_iBuffer;
	DX11ConstantBuffer<CBDMatrix> m_cBuffer;
	DX11ConstantBuffer<CBDLight> m_cLightBuffer;
	DX11ConstantBuffer<CBDCamera> m_cCameraBuffer;
	DX11VertexShader m_vShader;
	bool m_padding[8] = { false };
	DX11PixelShader m_pShader;
	Texture m_tex;

	Mat4x4 m_modelMatrix = DirectX::XMMatrixIdentity();
	float4 m_scale = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 m_rotation = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 m_position = float4(0.0f, 0.0f, 0.0f, 1.0f);
};

