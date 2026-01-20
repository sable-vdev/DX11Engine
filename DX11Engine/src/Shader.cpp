#include "Shader.hpp"

DX11VertexShader::DX11VertexShader(ID3D11Device* device, const std::wstring& shaderPath, const std::wstring& shaderName, D3D11_INPUT_ELEMENT_DESC* layout, U32 numElements)
{
	Create(device, shaderPath, shaderName, layout, numElements);
}

void DX11VertexShader::Create(ID3D11Device* device, const std::wstring& shaderPath, const std::wstring& shaderName, D3D11_INPUT_ELEMENT_DESC* layout, U32 numElements)
{
	//strictness on deprecated syntax: honestly no idea if this is good or needed as it doesnt seem to have a huge performance impact
	U32 flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifndef NDEBUG
	flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	ComPtr<ID3DBlob> errorBlob;

	std::wstring file = shaderPath + shaderName;

	auto hr = D3DCompileFromFile(file.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", flags, 0, m_vertexBuffer.GetAddressOf(), &errorBlob);

	if (FAILED(hr))
	{
		LOG(static_cast<char*>(errorBlob->GetBufferPointer()));
		errorBlob->Release();

		ThrowIfFailed(hr);
		return;
	}

	size_t pos = shaderName.find(L".");
	std::wstring fileName = shaderName.substr(0, pos);

	std::filesystem::path exeDir;
	std::wstring path;
	{
		wchar_t exePath[MAX_PATH];
		GetModuleFileNameW(NULL, exePath, MAX_PATH);
		path = std::filesystem::path(exePath).parent_path();
		path += L"\\" + fileName + L".cso";
	}

	ThrowIfFailed(D3DWriteBlobToFile(m_vertexBuffer.Get(), path.c_str(), true));

	ThrowIfFailed(device->CreateVertexShader(m_vertexBuffer->GetBufferPointer(), m_vertexBuffer->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf()));

	ThrowIfFailed(device->CreateInputLayout(layout, numElements, m_vertexBuffer->GetBufferPointer(), m_vertexBuffer->GetBufferSize(), m_inputLayout.GetAddressOf()));
}

void DX11VertexShader::Bind(ID3D11DeviceContext* deviceContext) const
{
	if (deviceContext)
	{
		deviceContext->IASetInputLayout(m_inputLayout.Get());
		deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	}
}


DX11PixelShader::DX11PixelShader(ID3D11Device* device, const std::wstring& shaderPath, const std::wstring& shaderName)
{
	Create(device, shaderPath, shaderName);
}

void DX11PixelShader::Bind(ID3D11DeviceContext* deviceContext) const
{
	if (deviceContext)
	{
		deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	}
}

void DX11PixelShader::Create(ID3D11Device* device, const std::wstring& shaderPath, const std::wstring& shaderName)
{
	U32 flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifndef NDEBUG
	flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ComPtr<ID3DBlob> errorBlob;

	std::wstring file = shaderPath + shaderName;

	auto hr = D3DCompileFromFile(file.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", flags, 0, m_pixelBuffer.GetAddressOf(), &errorBlob);

	if (FAILED(hr))
	{
		LOG(static_cast<char*>(errorBlob->GetBufferPointer()));
		errorBlob->Release();

		ThrowIfFailed(hr);

		return;
	}

	size_t pos = shaderName.find(L".");
	std::wstring fileName = shaderName.substr(0, pos);

	std::filesystem::path exeDir;
	std::wstring path;
	{
		wchar_t exePath[MAX_PATH];
		GetModuleFileNameW(NULL, exePath, MAX_PATH);
		path = std::filesystem::path(exePath).parent_path();
		path += L"\\" + fileName + L".cso";
	}

	ThrowIfFailed(D3DWriteBlobToFile(m_pixelBuffer.Get(), path.c_str(), true));

	ThrowIfFailed(device->CreatePixelShader(m_pixelBuffer->GetBufferPointer(), m_pixelBuffer->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf()));
}