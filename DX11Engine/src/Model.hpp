#pragma once
#include "Mesh.hpp"
#include "Texture.hpp"

class Model
{
public:
	Model() = delete;
	Model(ID3D11Device* device);
	void Draw(ID3D11DeviceContext* context) const;
	void Update(float dt);

	void PushToGpu(ID3D11Device* device);

	inline float3& GetPosition() { return m_position; }
	inline float3& GetRotation() { return m_rotation; }
	inline float3& GetScale() { return m_scale; }

private:
	DX::XMMATRIX m_modelMatrix;
public:
	std::string name;
	std::vector<Mesh> meshes;
	std::unique_ptr<DX11VertexShader> vertexShader;
	std::unique_ptr<DX11PixelShader> pixelShader;
	std::unique_ptr<ModelTexture> textures;
private:
	DX11ConstantBuffer<CBDMatrix> m_cBuffer;
	float3 m_position;
	float3 m_rotation = float3(0.0f, 0.0f, 0.0f);
	float3 m_scale = float3(1.0f, 1.0f, 1.0f);
};
