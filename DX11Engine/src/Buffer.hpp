#pragma once
#include <d3d11_4.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "DXHelper.hpp"
#include "EngineTypes.hpp"

class DX11VertexBuffer
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	DX11VertexBuffer(ID3D11Device* device, const void* data, U32 size, D3D11_USAGE usage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
	~DX11VertexBuffer() { m_buffer.Reset(); m_buffer = nullptr; };

	/*
	* Binds the buffer
	*/
	void Bind(ID3D11DeviceContext* context) const;
	void Draw(ID3D11DeviceContext* context) const;

private:
	ComPtr<ID3D11Buffer> m_buffer = nullptr;
};

class DX11IndexBuffer
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	DX11IndexBuffer(ID3D11Device* device, const void* data, U32 count, D3D11_USAGE usage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
	~DX11IndexBuffer() { m_buffer.Reset(); m_buffer = nullptr; };

	void Bind(ID3D11DeviceContext* context) const;
	void DrawIndexed(ID3D11DeviceContext* context) const;

	U32 GetIndexCount() const { return m_indexCount; }
private:
	ComPtr<ID3D11Buffer> m_buffer = nullptr;
	U32 m_indexCount;
};