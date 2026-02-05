#pragma once
#include <d3d11.h>
#include <vector>
#include <string>
#include <memory>

#include "Buffer.hpp"
#include "Shader.hpp"
#include "EngineTypes.hpp"
#include "ConstantBufferInfo.hpp"

struct Vertex
{
	float3 pos; 
	float3 normal;
	float2 texcoord;
};

struct Mesh
{
public: 
	Mesh(const std::vector<Vertex>& verts, const std::vector<U32>& indices, ID3D11Device* device);
	void Draw(ID3D11DeviceContext* context) const;
public:
	DX11VertexBuffer m_vertexBuffer;
	DX11IndexBuffer m_indexBuffer;

	U32 materialId = 0;
};