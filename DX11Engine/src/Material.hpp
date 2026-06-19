#pragma once
#include <memory>

#include "TextureManager.hpp"
#include "EngineTypes.hpp"
#include "Shader.hpp"

class Material
{
public:
	static Material CreateDefault(ID3D11Device* device)
	{
		Material mat;
		std::wstring path = L"..\\DX11Engine\\shaders\\";
		mat.vertexShader = std::make_unique<DX11VertexShader>(device, path, L"SimpleVShader.hlsl", VertexLayouts::PositionNormalTexcoord::Desc, VertexLayouts::PositionNormalTexcoord::Count);
		mat.pixelShader = std::make_unique<DX11PixelShader>(device, path, L"SimplePShader.hlsl");

		return mat;
	}

	void Bind(ID3D11DeviceContext* context, const ModelTexture& textures) const //, const ModelTexture& textures) const
	{
		vertexShader->Bind(context);
		pixelShader->Bind(context);

		if (auto tex = TextureManager::Get(textures.diffuse))
			tex->Bind(context);
	}

public:
	std::unique_ptr<DX11VertexShader> vertexShader;
	std::unique_ptr<DX11PixelShader> pixelShader;
};