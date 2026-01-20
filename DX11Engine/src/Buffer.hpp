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
	DX11VertexBuffer(ID3D11Device* device, const void* data, U32 stride, U32 size, D3D11_USAGE usage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
	/*
	* Binds the buffer
	*/
	void Bind(ID3D11DeviceContext* context) const;
	void Draw(ID3D11DeviceContext* context) const;

private:
	ComPtr<ID3D11Buffer> m_buffer = nullptr;
	U32 m_stride = 0;
};

class DX11IndexBuffer
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	DX11IndexBuffer(ID3D11Device* device, const void* data, U32 count, D3D11_USAGE usage = D3D11_USAGE::D3D11_USAGE_DEFAULT);

	void Bind(ID3D11DeviceContext* context) const;
	void DrawIndexed(ID3D11DeviceContext* context) const;

	U32 GetIndexCount() const { return m_indexCount; }
private:
	ComPtr<ID3D11Buffer> m_buffer = nullptr;
	U32 m_indexCount;
};

template<typename T>
class DX11ConstantBuffer
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	inline DX11ConstantBuffer(ID3D11Device* device)
	{
		D3D11_BUFFER_DESC cbDesc{};
		cbDesc.ByteWidth = sizeof(T);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		ThrowIfFailed(device->CreateBuffer(&cbDesc, nullptr, &m_buffer));
	}

	/*
	* Binds the buffer
	*/
	inline void BindVS(ID3D11DeviceContext* context, const T& data, U32 slot = 0) const
	{
		D3D11_MAPPED_SUBRESOURCE msbData{};

		context->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msbData);
		T* dptr = static_cast<T*>(msbData.pData);

		*dptr = data;

		context->Unmap(m_buffer.Get(), 0);

		context->VSSetConstantBuffers(slot, 1, m_buffer.GetAddressOf());
	}

	/*
	* Binds the buffer
	*/
	inline void BindPS(ID3D11DeviceContext* context, const T& data, U32 slot = 0) const
	{
		D3D11_MAPPED_SUBRESOURCE msbData{};

		context->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msbData);
		T* dptr = static_cast<T*>(msbData.pData);

		*dptr = data;

		context->Unmap(m_buffer.Get(), 0);

		context->PSSetConstantBuffers(slot, 1, m_buffer.GetAddressOf());
	}

private:
	ComPtr<ID3D11Buffer> m_buffer = nullptr;
};