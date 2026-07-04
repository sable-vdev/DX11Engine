#pragma once
#include <DirectXMath.h>
#include <filesystem>
#include <Windows.h>

static std::filesystem::path GetResourcePath()
{
#ifdef _DEBUG
	return std::filesystem::current_path();
#else
	wchar_t path[MAX_PATH];
	GetModuleFileNameW(nullptr, path, MAX_PATH);
	return std::filesystem::path(path).parent_path();
#endif
}

namespace DX = DirectX;

using I8 = signed char;
using I16 = short;
using I32 = int;
using I64 = long long;
using U8 = unsigned char;
using U16 = unsigned short;
using U32 = unsigned int;
using U64 = unsigned long long;

using Mat3 = DX::XMFLOAT3X3;
using Mat4 = DX::XMFLOAT4X4;

using float2 = DX::XMFLOAT2;
using float3 = DX::XMFLOAT3;
using float4 = DX::XMFLOAT4;

using Vec4 = DX::XMVECTOR;