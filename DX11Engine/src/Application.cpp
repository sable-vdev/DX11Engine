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
	m_camera = std::make_unique<Camera>(m_window->GetWidth(), m_window->GetHeight());
	m_timer = std::make_unique<Timer>();

	float data[] = {
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 1.0f,
		
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f,  0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f,

		0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
		0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 1.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f
	};

	U32 indices[] =
	{
		0,  1,  2,   0,  2,  3,
		4,  5,  6,   4,  6,  7,
		8,  9, 10,   8, 10, 11,
		12, 13, 14,  12, 14, 15,
		16, 17, 18,  16, 18, 19,
		20, 21, 22,  20, 22, 23 
	};

	DX11VertexBuffer* m_vBuffer = new DX11VertexBuffer(m_context->GetDevice(), data, sizeof(data));
	DX11IndexBuffer* m_iBuffer = new DX11IndexBuffer(m_context->GetDevice(), indices, sizeof(indices) / sizeof(indices[0]));
	DX11ConstantBuffer<CBDMatrix>* m_cBuffer = new DX11ConstantBuffer<CBDMatrix>(m_context->GetDevice());

	std::wstring path = L"..\\DX11Engine\\shaders\\";

	tex = new Texture(m_context->GetDevice(), "C:\\Dev\\DX11Engine\\DX11Engine\\resources\\texture.png");

	DX11VertexShader* m_vShader = new DX11VertexShader(m_context->GetDevice(), path, L"VertexShader.hlsl", VertexLayouts::PositionTexcoord::Desc, 2);
	DX11PixelShader* m_pShader = new DX11PixelShader(m_context->GetDevice(), path, L"PixelShader.hlsl");


	model1 = new Model(*m_vBuffer, *m_iBuffer, *m_cBuffer, *m_vShader, *m_pShader);

	delete m_iBuffer;
	delete m_vBuffer;
	delete m_pShader;
	delete m_vShader;
	delete m_cBuffer;
}

Application::~Application()
{
	delete tex;
	delete model1;
}

void Application::Run()
{
	while (true)
	{

		m_timer->Tick();

		static float rotation = 0.0f;
		rotation -= 0.0174532925f * 15.0f * m_timer->GetDeltaTime();

		if (rotation < 0.0f)
			rotation = 360.0f;

		m_timer->GetFramesPerSecond();

		if (!m_window->Run())
			break;

		m_camera->Update(m_timer->GetDeltaTime());

		m_context->BeginFrame();
		/*
		m_vBuffer->Bind(m_context->GetDeviceContext());
		auto matW = m_context->GetWorldMatrix();
		auto matV = m_camera->GetViewMatrix();
		auto matProj = m_camera->GetProjectionMatrix();

		matW = DirectX::XMMatrixRotationY(rotation);

		CBDMatrix data{};
		data.wvp = DirectX::XMMatrixMultiply(matW, matV);
		data.wvp = DirectX::XMMatrixTranspose(DirectX::XMMatrixMultiply(data.wvp, matProj));

		m_cBuffer->BindVS(m_context->GetDeviceContext(), data);

		m_vShader->Bind(m_context->GetDeviceContext());
		m_pShader->Bind(m_context->GetDeviceContext());

		tex->Bind(m_context->GetDeviceContext());
		m_iBuffer->DrawIndexed(m_context->GetDeviceContext());
		*/

		model1->Draw(m_context->GetDeviceContext());

		m_context->EndFrame(true);
	}
}

void Application::OnResize(U32 width, U32 height)
{
	if (m_context)
		m_context->OnResize(width, height);

	//if (m_camera)
		//m_camera->OnResize(width, height);
}
