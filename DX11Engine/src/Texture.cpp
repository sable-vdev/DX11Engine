#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(ID3D11Device* device, const std::string& file)
{

	auto tex = LoadFromFile(file);

	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = tex.width;
	desc.Height = tex.height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA subData{};
	subData.pSysMem = tex.pixels;
	subData.SysMemPitch = static_cast<UINT>(tex.width * 4);

	if (device)
	{
		ThrowIfFailed(device->CreateTexture2D(&desc, &subData, &m_texture));
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	ThrowIfFailed(device->CreateShaderResourceView(m_texture.Get(), &srvDesc, &m_shaderResourceView));

	stbi_image_free(tex.pixels);

	CreateSampler(device);
}

void Texture::Bind(ID3D11DeviceContext* deviceContext, U32 slot)
{
	if (deviceContext)
	{
		ID3D11ShaderResourceView* srv = m_shaderResourceView.Get();
		deviceContext->PSSetShaderResources(slot, 1, &srv);
		
		ID3D11SamplerState* samp = m_samplerState.Get();
		deviceContext->PSSetSamplers(slot, 1, &samp);
	}
}

Texture::TextureLoaded Texture::LoadFromFile(const std::string& file)
{
	TextureLoaded tex{};

	tex.pixels = stbi_load(file.c_str(), &tex.width, &tex.height, &tex.channels, 4);

	if (!tex.pixels)
	{
		LOG("Failed to load the texture");
		LOG(stbi_failure_reason());
		throw std::runtime_error("");
	}

	return tex;
}

void Texture::CreateSampler(ID3D11Device* device)
{
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ThrowIfFailed(device->CreateSamplerState(&samplerDesc, &m_samplerState));
}
