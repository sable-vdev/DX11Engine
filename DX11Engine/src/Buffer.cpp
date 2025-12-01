#include "Buffer.hpp"

DX11VertexBuffer::DX11VertexBuffer(ID3D11Device* device, const void* data, U32 size, D3D11_USAGE usage)
{
	D3D11_BUFFER_DESC bDesc{};
	bDesc.Usage = usage;
	bDesc.ByteWidth = size;
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bDesc.CPUAccessFlags = 0;
	bDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subData{};
	subData.pSysMem = data;
	subData.SysMemPitch = 0;
	subData.SysMemSlicePitch = 0;

	ThrowIfFailed(device->CreateBuffer(&bDesc, &subData, m_buffer.GetAddressOf()));
}

void DX11VertexBuffer::Bind(ID3D11DeviceContext* context) const
{
	if (context)
	{
		U32 stride = sizeof(float) * 8;
		U32 offset = 0;
		context->IASetVertexBuffers(0, 1, m_buffer.GetAddressOf(), &stride, &offset);
	}
}

void DX11VertexBuffer::Draw(ID3D11DeviceContext* context) const
{
	if (context)
	{
		Bind(context);
		context->Draw(4, 0);
	}
}

DX11IndexBuffer::DX11IndexBuffer(ID3D11Device* device, const void* data, U32 count, D3D11_USAGE usage) : m_indexCount(count)
{
	D3D11_BUFFER_DESC bDesc{};
	bDesc.Usage = usage;
	bDesc.ByteWidth = m_indexCount * sizeof(U32);
	bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bDesc.CPUAccessFlags = 0;
	bDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subData{};
	subData.pSysMem = data;
	subData.SysMemPitch = 0;
	subData.SysMemSlicePitch = 0;

	ThrowIfFailed(device->CreateBuffer(&bDesc, &subData, m_buffer.GetAddressOf()));
}

void DX11IndexBuffer::Bind(ID3D11DeviceContext* context) const
{
	if (context)
	{
		U32 offset = 0;
		context->IASetIndexBuffer(m_buffer.Get(), DXGI_FORMAT_R32_UINT, offset);
	}
}

void DX11IndexBuffer::DrawIndexed(ID3D11DeviceContext* context) const
{
	if (context)
	{
		Bind(context);
		context->DrawIndexed(m_indexCount, 0, 0);
	}
}
