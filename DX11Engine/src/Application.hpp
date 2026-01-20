#pragma once
#include <memory>
#include <vector>

#include "Window.hpp"
#include "DX11Context.hpp"
#include "Texture.hpp"
#include "Input.hpp"
#include "Camera.hpp"
#include "Timer.hpp"
#include "Model.hpp"
#include "ImGuiLayer.hpp"
#include "ObjectLoader.hpp"

class Application
{
public:
	Application(U32 width = 1280, U32 height = 720, const std::wstring& windowTitle = L"DX11 Engine", bool vsync = false);
	~Application();
	void Run();

	void OnResize(U32 width, U32 height);

	ID3D11Device* GetDevice() const 
	{ 
		if (m_context) 
			return m_context->GetDevice(); 
	}

	ID3D11DeviceContext* GetContext() const
	{
		if (m_context)
			return m_context->GetDeviceContext();
	}

	HWND GetHWND() const
	{
		if (m_window)
			return m_window->GetHWND();
	}

	U32 GetWidth() const
	{
		if (m_window)
			return m_window->GetWidth();
	}

	U32 GetHeight() const
	{
		if (m_window)
			return m_window->GetHeight();
	}

	bool& GetVsync() { return m_vsync; }

	void ToggleVsync(bool toggle) { m_vsync = toggle; }

	static inline Application& Get() { return *s_instance; }
private:
	static Application* s_instance;
	std::unique_ptr<Window> m_window;
	std::unique_ptr<DX11Context> m_context;
	std::unique_ptr<Input> m_input;
	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<Timer> m_timer;
	std::unique_ptr<ImGuiLayer> m_imguiLayer;
	std::vector<Model> m_models;
	bool m_vsync = false;

	bool m_padding[7];
	/*
	DX11VertexBuffer* m_vBuffer;
	DX11IndexBuffer* m_iBuffer;
	DX11ConstantBuffer<CBDMatrix>* m_cBuffer;
	DX11VertexShader* m_vShader;
	DX11PixelShader* m_pShader;
	*/
	//Texture* tex;
};

