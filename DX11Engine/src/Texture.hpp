#pragma once
#include <d3d11_4.h>
#include <wrl/client.h>

#include "DXHelper.hpp"
#include "EngineTypes.hpp"

//For now we only accept textures(pics) with a standardized size of 256x256

class Texture
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	Texture(ID3D11Device* device);

private:
	ComPtr<ID3D11Texture2D> m_texture;
};

