#include "Application.hpp"
#include "RendererQueue.hpp"

Application* Application::s_instance = nullptr;

Application::Application(U32 width, U32 height, const std::wstring& windowTitle, bool vsync) : m_vsync(false)
{
	if (s_instance) 
		LOG("Instance of application already there");

	s_instance = this;

	m_window = std::make_unique<Window>(width, height, windowTitle);

	m_context = std::make_unique<DX11Context>(m_window->GetHWND());

	m_context->Init();

	m_input = std::make_unique<Input>();
	m_camera = std::make_unique<Camera>(m_window->GetWidth(), m_window->GetHeight());
	m_timer = std::make_unique<Timer>();


	m_imguiLayer = std::make_unique<ImGuiLayer>();

	ObjectLoader::LoadObjectAsync("C:\\Dev\\DX11Engine\\DX11Engine\\resources\\backpack\\backpack.obj", m_modelQueue);
	ObjectLoader::LoadObjectAsync("C:\\Dev\\DX11Engine\\DX11Engine\\resources\\backpack\\backpack.obj", m_modelQueue);
	//ObjectLoader::LoadObjectAsync("C:\\Dev\\DX11Engine\\DX11Engine\\resources\\teapot.obj", m_modelQueue);
	//sprite = new Sprite("C:\\Dev\\DX11Engine\\DX11Engine\\resources\\texture.png");
}

Application::~Application()
{
	delete sprite;
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


		std::unique_ptr<Model> model;
		while (m_modelQueue.TryPop(model))
		{
			m_models.push_back(std::move(model));
		}

		for (const auto& model : m_models)
			model->Update(m_timer->GetDeltaTime());

		RendererQueue::AddContext(m_context->GetDeviceContext());

		m_context->BeginFrame();

		for (const auto& model : m_models)
		{
			RendererQueue::Enqueue(model.get());
		}

		RendererQueue::Flush();
		
		m_imguiLayer->Begin();

		if (m_models.size() > 0)
			m_imguiLayer->ModelInfo(*m_models[0]);

		m_imguiLayer->Render();

		m_imguiLayer->End();

		m_context->EndFrame(m_vsync);
	}
}

void Application::OnResize(U32 width, U32 height)
{
	if (m_context)
		m_context->OnResize(width, height);

	if (m_camera)
		m_camera->OnResize(width, height);
}
