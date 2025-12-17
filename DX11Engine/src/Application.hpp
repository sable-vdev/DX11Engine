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

class Application
{
public:
	Application(U32 width = 1280, U32 height = 720, const std::wstring& windowTitle = L"DX11 Engine");
	~Application();
	void Run();

	void OnResize(U32 width, U32 height);

	static inline Application& Get() { return *s_instance; }
private:
	static Application* s_instance;
	std::unique_ptr<Window> m_window;
	std::unique_ptr<DX11Context> m_context;
	std::unique_ptr<Input> m_input;
	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<Timer> m_timer;
	Model* model1;
	/*
	DX11VertexBuffer* m_vBuffer;
	DX11IndexBuffer* m_iBuffer;
	DX11ConstantBuffer<CBDMatrix>* m_cBuffer;
	DX11VertexShader* m_vShader;
	DX11PixelShader* m_pShader;
	*/
	//Texture* tex;
};

