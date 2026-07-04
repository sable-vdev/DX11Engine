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

		MessageBoxW(nullptr, GetResourcePath().c_str(), L"Debug File Path", MB_OKCANCEL);
		std::filesystem::path shaderPath = GetResourcePath() / "shaders";
		mat.vertexShader = std::make_unique<DX11VertexShader>(device, shaderPath, L"SimpleVShader.hlsl", VertexLayouts::PositionNormalTexcoord::Desc, VertexLayouts::PositionNormalTexcoord::Count);
		mat.pixelShader = std::make_unique<DX11PixelShader>(device, shaderPath, L"SimplePShader.hlsl");

		return mat;
	}

	void Bind(ID3D11DeviceContext* context, const ModelTexture& textures) const //, const ModelTexture& textures) const
	{
		vertexShader->Bind(context);
		pixelShader->Bind(context);

		if (auto tex = TextureManager::Get(textures.diffuse))
		{
			if(tex != nullptr)
				tex->Bind(context, 0);
		}

		if (auto tex = TextureManager::Get(textures.normal))
		{
			if (tex != nullptr)
				tex->Bind(context, 1);
		}

		if (auto tex = TextureManager::Get(textures.specular))
		{
			if (tex != nullptr)
				tex->Bind(context, 2);
		}
	}

public:
	std::unique_ptr<DX11VertexShader> vertexShader;
	std::unique_ptr<DX11PixelShader> pixelShader;
};