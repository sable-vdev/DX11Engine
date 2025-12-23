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


	m_imguiLayer = std::make_unique<ImGuiLayer>();

	float data[] = {
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,

		0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f
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

	DX11VertexBuffer vb = DX11VertexBuffer(m_context->GetDevice(), data, VertexLayouts::PositionTexcoordNormal::Stride, sizeof(data));
	DX11IndexBuffer ib = DX11IndexBuffer(m_context->GetDevice(), indices, sizeof(indices) / sizeof(indices[0]));
	DX11ConstantBuffer<CBDMatrix> cb = DX11ConstantBuffer<CBDMatrix>(m_context->GetDevice());
	DX11ConstantBuffer<CBDLight> clb = DX11ConstantBuffer<CBDLight>(m_context->GetDevice());
	DX11ConstantBuffer<CBDCamera> clCam = DX11ConstantBuffer<CBDCamera>(m_context->GetDevice());
	
	std::wstring path = L"..\\DX11Engine\\shaders\\";
	
	Texture tex = Texture(m_context->GetDevice(), "C:\\Dev\\DX11Engine\\DX11Engine\\resources\\container2.png");
	
	DX11VertexShader vs = DX11VertexShader(m_context->GetDevice(), path, L"VertexShader.hlsl", VertexLayouts::PositionTexcoordNormal::Desc, VertexLayouts::PositionTexcoordNormal::Count);
	DX11PixelShader ps = DX11PixelShader(m_context->GetDevice(), path, L"PixelShader.hlsl");

	model1 = new Model(vb, ib, cb, clb, clCam, vs, ps, tex);
}

Application::~Application()
{
}

void Application::Run()
{
	while (true)
	{

		m_timer->Tick();

		m_timer->GetFramesPerSecond();

		if (!m_window->Run())
			break;

		m_camera->Update(m_timer->GetDeltaTime());

		model1->Update(m_timer->GetDeltaTime());

		m_context->BeginFrame();

		model1->Render(m_context->GetDeviceContext());


		m_imguiLayer->Render();

		m_context->EndFrame(true);
	}
}

void Application::OnResize(U32 width, U32 height)
{
	if (m_context)
		m_context->OnResize(width, height);

	if (m_camera)
		m_camera->OnResize(width, height);
}
