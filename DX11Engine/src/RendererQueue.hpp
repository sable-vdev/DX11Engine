#pragma once
#include <memory>

#include "Model.hpp"
#include "wrl/client.h"

class RendererQueue
{
public:
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	static void AddContext(const ComPtr<ID3D11DeviceContext>& deviceContext);
	static void Enqueue(const Model* model);
	static void Flush();
private:
	static ComPtr<ID3D11DeviceContext> s_deviceContext;
	static std::vector<const Model*> s_queue;
};