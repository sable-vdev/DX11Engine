#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& file)
{
	m_cpuInfo = LoadFromFile(file);
}

Texture::~Texture()
{
	if (m_shaderResourceView)
		m_shaderResourceView.Reset();

	if (m_samplerState)
		m_samplerState.Reset();
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

void Texture::UploadToGpu(ID3D11Device* device)
{
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = m_cpuInfo.width;
	desc.Height = m_cpuInfo.height;
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
	subData.pSysMem = m_cpuInfo.pixels;
	subData.SysMemPitch = static_cast<UINT>(m_cpuInfo.width * 4);

	if (device)
	{
		ThrowIfFailed(device->CreateTexture2D(&desc, &subData, &m_texture));
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	ThrowIfFailed(device->CreateShaderResourceView(m_texture.Get(), &srvDesc, &m_shaderResourceView));

	stbi_image_free(m_cpuInfo.pixels);
	m_cpuInfo.pixels = nullptr;

	isUploaded = true;

	CreateSampler(device);
}

Texture::CPUTex Texture::LoadFromFile(const std::string& file)
{
	CPUTex tex{};

	//stbi_set_flip_vertically_on_load(true);

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
