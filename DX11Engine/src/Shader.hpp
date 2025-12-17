#pragma once

#pragma comment(lib, "D3DCompiler.lib")
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <string>
#include <filesystem>

#include "EngineTypes.hpp"
#include "DXHelper.hpp"
#include "Logger.hpp"

//using this for now, its simpler
namespace VertexLayouts
{
	namespace Position
	{
		static D3D11_INPUT_ELEMENT_DESC Desc[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		static U32 Count = 1;
		static U32 Stride = 12;
	};

	namespace PositionColor
	{
		static D3D11_INPUT_ELEMENT_DESC Desc[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		static U32 Count = 2;
		static U32 Stride = 24;
	};

	namespace PositionTexcoord
	{
		static D3D11_INPUT_ELEMENT_DESC Desc[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		static U32 Count = 2;
		static U32 Stride = 20;
	};

	namespace PositionColorTexcoord
	{
		static D3D11_INPUT_ELEMENT_DESC Desc[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		static U32 Count = 3;
		static U32 Stride = 32;
	}

	namespace PositionTexcoordNormal
	{
		static D3D11_INPUT_ELEMENT_DESC Desc[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		static U32 Count = 3;
		static U32 Stride = 32;
	}
};

class DX11VertexShader
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	
public:
	DX11VertexShader(ID3D11Device* device, const std::wstring& shaderPath, const std::wstring& shaderName, D3D11_INPUT_ELEMENT_DESC* layout, U32 numElements);
	void Bind(ID3D11DeviceContext* deviceContext);

	//ID3D11VertexShader* GetShader() const { return m_vertexShader.Get(); }
	//ID3D11InputLayout* GetLayout() const { return m_inputLayout.Get(); }
private:
	void Create(ID3D11Device* device, const std::wstring& shaderPath, const std::wstring& shaderName, D3D11_INPUT_ELEMENT_DESC* layout, U32 numElements);
private:
	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3DBlob> m_vertexBuffer;
	ComPtr<ID3D11InputLayout> m_inputLayout;
	//ComPtr<D3D11_MAPPED_SUBRESOURCE> m_subresources;
};

class DX11PixelShader
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	DX11PixelShader(ID3D11Device* device, const std::wstring& shaderPath, const std::wstring& shaderName);
	void Bind(ID3D11DeviceContext* deviceContext);
	//ID3D11PixelShader* GetShader() const { return m_pixelShader.Get(); }
private:
	void Create(ID3D11Device* device, const std::wstring& shaderPath, const std::wstring& shaderName);
private:
	ComPtr<ID3D11PixelShader> m_pixelShader;
	//using ID3Dblob since its version neutral (as per: https://learn.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ff728743(v=vs.85)
	ComPtr<ID3DBlob> m_pixelBuffer;
};

