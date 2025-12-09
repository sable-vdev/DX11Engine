#pragma once
#include <d3d11_4.h>
#include <wrl/client.h>

#include "DXHelper.hpp"
#include "EngineTypes.hpp"
#include "Logger.hpp"

//For now we only accept textures(pics) with a standardized size of 256x256

class Texture
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	struct TextureLoaded
	{
		U8* pixels;
		I32 width, height, channels;

		inline ~TextureLoaded()
		{
			//delete pixels;
		}
	};

public:
	Texture(ID3D11Device* device, const std::string& file);
	~Texture();
	void Bind(ID3D11DeviceContext* deviceContext, U32 slot = 0);
private:
	TextureLoaded LoadFromFile(const std::string& file);
	void CreateSampler(ID3D11Device* device);
private:
	ComPtr<ID3D11Texture2D> m_texture;
	ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
	ComPtr<ID3D11SamplerState> m_samplerState;
};

