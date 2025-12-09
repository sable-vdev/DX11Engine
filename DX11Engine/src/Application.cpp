#include "Application.hpp"

Application* Application::s_instance = nullptr;

Application::Application(U32 width, U32 height, const std::wstring& windowTitle)
{
	if (s_instance) LOG("Instance of application already there");

	s_instance = this;

	m_window = std::make_unique<Window>(width, height, windowTitle);

	m_context = std::make_unique<DX11Context>(m_window->GetHWND());

	m_context->Init();

	m_input = std::make_unique<Input>();
	m_camera = std::make_unique<Camera>();

	float data[] = {
			-0.5f, -0.5f, .0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, .0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f, 0.5f, .0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, .0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f
	};

	U32 indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	m_vBuffer = new DX11VertexBuffer(m_context->GetDevice(), data, sizeof(data));
	m_iBuffer = new DX11IndexBuffer(m_context->GetDevice(), indices, sizeof(indices) / sizeof(indices[0]));
	m_cBuffer = new DX11ConstantBuffer<CBDMatrix>(m_context->GetDevice());

	std::wstring path = L"..\\DX11Engine\\shaders\\";

	tex = new Texture(m_context->GetDevice(), "C:\\Dev\\DX11Engine\\DX11Engine\\resources\\texture.png");

	m_vShader = new DX11VertexShader(m_context->GetDevice(), path, L"VertexShader.hlsl", VertexLayouts::PositionColorTexcoord::Desc, ARRAYSIZE(VertexLayouts::PositionColorTexcoord::Desc));
	m_pShader = new DX11PixelShader(m_context->GetDevice(), path, L"PixelShader.hlsl");
}

Application::~Application()
{
	delete m_iBuffer;
	delete m_vBuffer;
	delete m_pShader;
	delete m_vShader;
	delete tex;
}

void Application::Run()
{
	while (true)
	{
		if (!m_window->Run())
			break;

		m_context->BeginFrame();

		m_camera->Update();

		m_vBuffer->Bind(m_context->GetDeviceContext());

		auto matW = m_context->GetWorldMatrix();
		auto matV = m_camera->GetViewMatrix();
		auto matProj = m_context->GetProjectionMatrix();

		CBDMatrix data{};
		data.wvp = DirectX::XMMatrixMultiply(matW, matV);
		data.wvp = DirectX::XMMatrixTranspose(DirectX::XMMatrixMultiply(data.wvp, matProj));

		m_cBuffer->BindVS(m_context->GetDeviceContext(), data);

		m_vShader->Bind(m_context->GetDeviceContext());
		m_pShader->Bind(m_context->GetDeviceContext());

		tex->Bind(m_context->GetDeviceContext());
		m_iBuffer->DrawIndexed(m_context->GetDeviceContext());

		m_context->EndFrame();
	}
}

void Application::OnResize(U32 width, U32 height)
{
	if (m_context)
		m_context->OnResize(width, height);
}
