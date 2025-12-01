#include "Texture.hpp"

Texture::Texture(ID3D11Device* device)
{
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = 256;
	desc.Height = 256;
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
	subData.pSysMem = nullptr;
	//rowpitch = width * channels (4rgba) * sizeof(datatype)
	U32 rowPitch = (256 * 4) * sizeof(U8);
	subData.SysMemPitch = static_cast<UINT>(rowPitch);
	subData.SysMemSlicePitch = static_cast<UINT>(rowPitch * 256); //rowpitch * height (https://learn.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-resources-textures-how-to)

	if (device)
	{
		ThrowIfFailed(device->CreateTexture2D(&desc, &subData, &m_texture));
	}
}
