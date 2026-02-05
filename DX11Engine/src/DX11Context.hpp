#pragma once

#include <d3d11_4.h>
#include <wrl/client.h>
#include <dxgi1_6.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include "EngineTypes.hpp"
#include "Logger.hpp"
#include "DXHelper.hpp"

class DX11Context
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	DX11Context(HWND hwnd);
	~DX11Context();

	void Init();

	void SetViewport(U32 width, U32 height);

	void BeginFrame();
	void EndFrame(bool vsnyc = false);

	void OnResize(U32 width, U32 height);

	void ToggleDepth(bool enabled = true);

	ID3D11Device* GetDevice() const { return m_device.Get(); }
	ID3D11DeviceContext* GetDeviceContext() const { return m_deviceContext.Get(); }

	Mat4x4 GetWorldMatrix() const { return m_worldMatrix; }
private:
	void CreateDeviceAndSwapChain(HWND hwnd);
	void CreateRenderTargetView();
	void CreateDepthStencilView(U32 width, U32 height);
	void CreateRasterizerState();
	void SetRenderTarget();
private:
	HWND m_hwnd;
	ComPtr<ID3D11Device5> m_device;
	ComPtr<ID3D11DeviceContext4> m_deviceContext;
	ComPtr<IDXGISwapChain4> m_swapChain;
	ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	ComPtr<ID3D11DepthStencilState> m_depthStencilState;
	ComPtr<ID3D11DepthStencilState> m_depthDisabledStencilState;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ComPtr<ID3D11RasterizerState2> m_rasterizerState;
	Mat4x4 m_worldMatrix;
	bool m_allowTearing = false;
};

