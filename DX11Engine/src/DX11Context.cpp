#include "DX11Context.hpp"

DX11Context::DX11Context(HWND hwnd) : m_hwnd(hwnd), m_worldMatrix()
{
}

DX11Context::~DX11Context()
{


	if (m_deviceContext)
	{
		m_deviceContext->ClearState();
		m_deviceContext->Flush();
	}

	m_renderTargetView.Reset();
	m_depthStencilState.Reset();
	m_depthStencilView.Reset();
	m_rasterizerState.Reset();
	m_swapChain.Reset();

	m_deviceContext.Reset();

#ifndef NDEBUG
	if (m_device)
	{
		ComPtr<ID3D11InfoQueue> infoQue;
		if (SUCCEEDED(m_device.As(&infoQue)))
		{
			infoQue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, FALSE);
			infoQue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, FALSE);
			infoQue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, FALSE);

			infoQue->ClearStoredMessages();
		}
	}
#endif //!NDEBUG

	m_device.Reset();
}

void DX11Context::Init()
{
	CreateDeviceAndSwapChain(m_hwnd);
	CreateRenderTargetView();

	U32 width = 0, height = 0;
	RECT r{};
	GetClientRect(m_hwnd, &r);
	width = r.right - r.left;
	height = r.bottom - r.top;

	CreateDepthStencilView(width, height);
	CreateRasterizerState();

	SetRenderTarget();

	m_worldMatrix = DirectX::XMMatrixIdentity();
}

void DX11Context::CreateDeviceAndSwapChain(HWND hwnd)
{
	U32 flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifndef NDEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	ComPtr<IDXGIFactory7> factory;
	ThrowIfFailed(CreateDXGIFactory2(0, IID_PPV_ARGS(&factory)));

	BOOL allow = FALSE;
	ThrowIfFailed(factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allow, sizeof(allow)));
	m_allowTearing = allow == TRUE;

	ComPtr<IDXGIAdapter4> adapter;
	ThrowIfFailed(factory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)));

	DXGI_ADAPTER_DESC3 desc;
	adapter->GetDesc3(&desc);

	std::wstring description = desc.Description;

	LOG_W(description.c_str());

	I32 width = 0, height = 0;
	RECT r{};
	GetClientRect(hwnd, &r);
	width = r.right - r.left;
	height = r.bottom - r.top;

	DXGI_SWAP_CHAIN_DESC1 scDesc{};
	scDesc.BufferCount = 2;
	scDesc.Width = width;
	scDesc.Height = height;
	scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scDesc.SampleDesc.Count = 1;
	scDesc.SampleDesc.Quality = 0;
	scDesc.Scaling = DXGI_SCALING_STRETCH;
	scDesc.Stereo = FALSE;
	scDesc.Flags = m_allowTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
	ComPtr<IDXGISwapChain1> swapChain = nullptr;
	ComPtr<ID3D11Device> device = nullptr;
	ComPtr<ID3D11DeviceContext> deviceContext = nullptr;

	ThrowIfFailed(D3D11CreateDevice(adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr, flags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &device, nullptr, &deviceContext));

	ThrowIfFailed(factory->CreateSwapChainForHwnd(device.Get(), hwnd, &scDesc, nullptr, nullptr, &swapChain));

	//query interfaces of older versions to higher, if succeeded then delete older and use newer 
	if (SUCCEEDED(swapChain.As(&m_swapChain)))
	{
		swapChain.Reset();
	}
	if (SUCCEEDED(device.As(&m_device)))
	{
		device.Reset();
	}
	if (SUCCEEDED(deviceContext.As(&m_deviceContext)))
	{
		deviceContext.Reset();
	};

	//disableing alt+enter fullscreen toggle
	ThrowIfFailed(factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));

	factory.Reset();
	adapter.Reset();

	SetViewport(width, height);
}

void DX11Context::CreateRenderTargetView()
{
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;

	ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));

	ThrowIfFailed(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_renderTargetView));

	backBuffer.Reset();
}

void DX11Context::CreateDepthStencilView(U32 width, U32 height)
{
	ComPtr<ID3D11Texture2D> depthStencil = nullptr;

	D3D11_TEXTURE2D_DESC texDesc{};
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ThrowIfFailed(m_device->CreateTexture2D(&texDesc, nullptr, depthStencil.GetAddressOf()));

	D3D11_DEPTH_STENCIL_DESC depthDesc{};
	//depth test params
	depthDesc.DepthEnable = true;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;

	//stencil test params
	depthDesc.StencilEnable = true;
	depthDesc.StencilReadMask = 0xFF;
	depthDesc.StencilWriteMask = 0xFF;

	//tencil operations if pixel is front-facing
	depthDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Stencil operations if pixel is back-facing
	depthDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	ThrowIfFailed(m_device->CreateDepthStencilState(&depthDesc, &m_depthStencilState));

	//set state
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	ThrowIfFailed(m_device->CreateDepthStencilView(depthStencil.Get(), &dsvDesc, m_depthStencilView.GetAddressOf()));

	SetRenderTarget();

	if(depthStencil)
		depthStencil.Reset();
}

void DX11Context::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC2 rasDesc{};
	rasDesc.FillMode = D3D11_FILL_SOLID;
	rasDesc.CullMode = D3D11_CULL_NONE;
	rasDesc.FrontCounterClockwise = TRUE;
	rasDesc.DepthBias = 0;
	rasDesc.DepthBiasClamp = 0.0f;
	rasDesc.SlopeScaledDepthBias = 0.0f;
	rasDesc.DepthClipEnable = TRUE;
	rasDesc.ScissorEnable = FALSE;
	rasDesc.MultisampleEnable = FALSE;
	rasDesc.AntialiasedLineEnable = FALSE;

	ThrowIfFailed(m_device->CreateRasterizerState2(&rasDesc, &m_rasterizerState));
}

void DX11Context::SetViewport(U32 width, U32 height)
{
	D3D11_VIEWPORT vp{};
	vp.Width = static_cast<float>(width);
	vp.Height = static_cast<float>(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	if (m_deviceContext)
		m_deviceContext->RSSetViewports(1, &vp);
}

void DX11Context::SetRenderTarget()
{
	if (m_deviceContext)
		m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
}

void DX11Context::BeginFrame()
{
	float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f};
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	SetRenderTarget();
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);
	m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_deviceContext->RSSetState(m_rasterizerState.Get());
}

void DX11Context::EndFrame(bool vsync)
{
	if(vsync)
		ThrowIfFailed(m_swapChain->Present(1, 0));
	else
		ThrowIfFailed(m_swapChain->Present(0, m_allowTearing ? DXGI_PRESENT_ALLOW_TEARING : 0));
}

void DX11Context::OnResize(U32 width, U32 height)
{
	if (!m_swapChain || !m_deviceContext || !m_device || width == 0 || height == 0)
		return;

	m_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);

	m_renderTargetView.Reset();
	m_depthStencilView.Reset();

	//first parameter 0 to preserve existing number of buffers, same for third parameter
	ThrowIfFailed(m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, m_allowTearing ? DXGI_PRESENT_ALLOW_TEARING : 0));

	CreateRenderTargetView();
	CreateDepthStencilView(width, height);

	SetRenderTarget();

	SetViewport(width, height);
}
