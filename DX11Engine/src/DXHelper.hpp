#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdexcept>

inline bool ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
		throw std::runtime_error("");

	return false;
}