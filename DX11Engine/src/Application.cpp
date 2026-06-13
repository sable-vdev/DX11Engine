#include "Application.hpp"
#include "RendererQueue.hpp"

Application* Application::s_instance = nullptr;

Application::Application(U32 width, U32 height, const std::wstring& windowTitle, bool vsync) : m_vsync(false)
{
	Logger::Init();

	if (s_instance)
		LOG_ERROR("Instance of application already there");

	s_instance = this;

	m_window = std::make_unique<Window>(width, height, windowTitle);

	m_context = std::make_unique<DX11Context>(m_window->GetHWND());

	m_context->Init();

	m_input = std::make_unique<Input>();
	m_camera = std::make_unique<Camera>(m_window->GetWidth(), m_window->GetHeight());
	m_timer = std::make_unique<Timer>();

	m_scene = std::make_unique<Scene>();

	m_imguiLayer = std::make_unique<ImGuiLayer>();

	//ObjectLoader::LoadObjectAsync("C:\\Dev\\DX11Engine\\DX11Engine\\resources\\backpack\\backpack.obj", m_modelQueue);
	ObjectLoader::LoadObjectAsync("C:\\Dev\\DX11Engine\\DX11Engine\\resources\\backpack\\backpack.obj", m_modelQueue);

	LightData data;
	data.position = float4(0, 0, 0, 1);
	data.direction = float4(-0.5f, -1.0f, -0.5f, 0);
	data.ambient = float4(0.1f, 0.1f, 0.1f, 1);
	data.diffuse = float4(1, 1, 1, 1);
	data.specular = float4(1, 1, 1, 1);
	data.specularPower = 32.0f;
	m_scene->lightManager.AddLight(LightEntity{data});
}

Application::~Application()
{
}

void Application::Run()
{
	while (m_window->Run())
	{
		m_timer->Tick();

		m_timer->GetFramesPerSecond();

		m_camera->Update(m_timer->GetDeltaTime());


		std::unique_ptr<Model> model;
		while (m_modelQueue.TryPop(model))
		{
			m_models.push_back(std::move(model));
		}

		for (const auto& model : m_models)
			model->Update(m_timer->GetDeltaTime());

		RendererQueue::AddContext(m_context->GetDeviceContext());

		m_context->BeginFrame(m_context->GetViewportRTV());

		for (const auto& model : m_models)
		{
			RendererQueue::Enqueue(model.get());
		}

		RendererQueue::Flush(m_scene.get());

		m_context->BeginFrame(m_context->GetBackbufferRTV());

		m_imguiLayer->Begin();

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
